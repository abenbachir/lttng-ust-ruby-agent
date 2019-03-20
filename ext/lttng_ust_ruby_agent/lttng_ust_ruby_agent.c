#include "lttng_ust_ruby_agent.h"

#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE
#include "lttng_provider/lttng_ust_ruby_log.h"
#include "lttng_provider/lttng_ust_cyg_profile.h"
#include "lttng_provider/lttng_ust_ruby.h"


VALUE ruby_lttng_trace_debug(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, debug, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_info(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, info, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_notice(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, notice, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_warning(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, warning, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_err(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, err, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_crit(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, crit, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_alert(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, alert, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}

VALUE ruby_lttng_trace_emerg(VALUE self, VALUE mesg)
{
   tracepoint(lttng_ruby_log, emerg, rb_sourcefile(), rb_sourceline(), StringValuePtr(mesg));
   return(Qnil);
}
//TP_ARGS(unsigned long, func_addr, char*, func_name, unsigned long, gc_stat_count, long unsigned int, rss_kb)
VALUE ruby_lttng_trace_func_entry(VALUE self, VALUE fn, VALUE fn_name, VALUE gc_stat_count, VALUE rss_kb)
{
   tracepoint(lttng_ust_cyg_profile, func_entry, NUM2LONG(fn), StringValuePtr(fn_name), NUM2LONG(gc_stat_count), NUM2LONG(rss_kb));
   return(Qnil);
}

VALUE ruby_lttng_trace_func_exit(VALUE self, VALUE fn, VALUE fn_name, VALUE gc_stat_count, VALUE rss_kb)
{
   tracepoint(lttng_ust_cyg_profile, func_exit, NUM2LONG(fn), StringValuePtr(fn_name), NUM2LONG(gc_stat_count), NUM2LONG(rss_kb));
   return(Qnil);
}

VALUE ruby_lttng_trace_event(VALUE self, VALUE lineno, VALUE rss_kb, VALUE event, VALUE class_name, VALUE func_name, VALUE path)
{
   tracepoint(lttng_ruby, event, NUM2LONG(lineno), NUM2LONG(rss_kb), StringValuePtr(event), StringValuePtr(class_name),
        StringValuePtr(func_name), StringValuePtr(path));
   return(Qnil);
}

static int inside_gc = 0;
static int collect_gc_stat = 0;
static void gc_event_hook(VALUE tpval, void *data)
{
    rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
    rb_event_flag_t flag = rb_tracearg_event_flag(tparg);
    VALUE obj, line, klass, flags, path, mid;
    char *class_name = "";
    char *path_str = "";
    char *method_str = "";
    switch (flag)
    {
        case RUBY_INTERNAL_EVENT_GC_START:
//            printf("RUBY_INTERNAL_EVENT_GC_START\n");
            tracepoint(lttng_ruby, gc_start);
            return;
        case RUBY_INTERNAL_EVENT_GC_END_MARK:
//            printf("RUBY_INTERNAL_EVENT_GC_END_MARK\n");
            tracepoint(lttng_ruby, gc_end_mark);
            break;
        case RUBY_INTERNAL_EVENT_GC_END_SWEEP:
//            printf("RUBY_INTERNAL_EVENT_GC_END_SWEEP\n");
            tracepoint(lttng_ruby, gc_end_sweep);
            break;
        case RUBY_INTERNAL_EVENT_NEWOBJ:
            obj = rb_tracearg_object(tparg);
            line = rb_tracearg_lineno(tparg);
            klass = RBASIC_CLASS(obj);
            flags = RBASIC(obj)->flags;
            path = rb_tracearg_path(tparg);
            path_str = RTEST(path) ? RSTRING_PTR(path) : "";
            mid = rb_tracearg_method_id(tparg);
            method_str = RTEST(mid) ? RSTRING_PTR(rb_sym2str(mid)) : "";

            switch (BUILTIN_TYPE(obj)) {
                case T_CLASS:
                case T_MODULE:
                class_name = rb_obj_classname(obj);
                break;
            }

            tracepoint(lttng_ruby, newobj, (st_data_t)obj, flags, NUM2INT(line), obj_type(obj), class_name, path_str, method_str);
            break;
        case RUBY_INTERNAL_EVENT_FREEOBJ:
            obj = rb_tracearg_object(tparg);
            flags = RBASIC(obj)->flags;
            tracepoint(lttng_ruby, freeobj, (st_data_t)obj, flags, obj_type(obj));
            break;

#ifdef RUBY_INTERNAL_EVENT_GC_ENTER
        case RUBY_INTERNAL_EVENT_GC_ENTER:
            inside_gc++;
//            printf("RUBY_INTERNAL_EVENT_GC_ENTER=%d\n", inside_gc);
            tracepoint(lttng_ruby, gc_enter);
            break;
        case RUBY_INTERNAL_EVENT_GC_EXIT:
            inside_gc--;
            collect_gc_stat = 1;
//            printf("RUBY_INTERNAL_EVENT_GC_EXIT=%d\n", inside_gc);
            tracepoint(lttng_ruby, gc_exit);
            break;
#endif
    }

    switch (flag)
    {
        case RUBY_INTERNAL_EVENT_GC_START:
        case RUBY_INTERNAL_EVENT_GC_END_MARK:
        case RUBY_INTERNAL_EVENT_GC_END_SWEEP:
        case RUBY_INTERNAL_EVENT_NEWOBJ:
            if (inside_gc <= 0 && collect_gc_stat > 0)
            {
//                printf("GC_STAT\n");
                collect_gc_stat = 0;
                struct gc_stat_entry stat = get_gc_stat_entry();
                tracepoint(lttng_ruby, gc_stat, &stat);
            }
            break;
    }
}

static int in_event_hook = 0;
static void event_hook(VALUE tpval, void *data)
{
    // do not re-enter this function
    // after this, must `goto out` instead of `return`
//    if (in_event_hook) return;
//        in_event_hook++;

    rb_trace_arg_t *tparg = rb_tracearg_from_tracepoint(tpval);
    rb_event_flag_t flag = rb_tracearg_event_flag(tparg);

    switch (flag)
    {
        case RUBY_EVENT_C_CALL:
        case RUBY_EVENT_CALL:
//        rb_tracearg_method_id(trace_arg)
            break;
        case RUBY_EVENT_C_RETURN:
        case RUBY_EVENT_RETURN:
            break;
        case RUBY_EVENT_THREAD_BEGIN:
            tracepoint(lttng_ruby, thread_begin);
            break;
        case RUBY_EVENT_THREAD_END:
            tracepoint(lttng_ruby, thread_end);
            break;
        case RUBY_EVENT_FIBER_SWITCH:
            tracepoint(lttng_ruby, fiber_switch);
            break;
        default:
            printf("unkown event: flag=%d\n", flag);
    }
}


VALUE ruby_lttng_enable_tracepoints(VALUE self)
{
    int i;
    printf("ruby_lttng_enable_tracepoints: \n");
    if (!NIL_P(tracepoints))
    {
        for (i = 0; i < RARRAY_LENINT(tracepoints); i++)
            rb_tracepoint_enable(rb_ary_entry(tracepoints, i));
        tracing_enabled = 1;
    }
    return(Qnil);
}

VALUE ruby_lttng_disable_tracepoints(VALUE self)
{
    int i;
    printf("ruby_lttng_disable_tracepoints: tracing_enabled=%d\n", tracing_enabled);
    if (!NIL_P(tracepoints) && tracing_enabled)
    {
        for (i = 0; i < RARRAY_LENINT(tracepoints); i++)
        {
            printf("i=%d\n", i);
            VALUE tp = rb_ary_entry(tracepoints, i);
            if (tp)
                rb_tracepoint_disable(tp);
        }
        tracing_enabled = 0;
    }
    return(Qnil);
}

VALUE ruby_lttng_register_tracepoints(VALUE self)
{
    int i = 0;
    printf("ruby_lttng_register_tracepoints: \n");
    if (NIL_P(tracepoints))
    {
        tracepoints = rb_ary_new();

        for (i = 0; i < ARRAY_SIZE(branch_trace_events); i++)
        {
            VALUE tp = rb_tracepoint_new(Qnil, branch_trace_events[i], event_hook, 0);
            rb_ary_push(tracepoints, tp);
        }

        for (i = 0; i < ARRAY_SIZE(gc_trace_events); i++)
        {

            VALUE tp = rb_tracepoint_new(Qnil, gc_trace_events[i], gc_event_hook, 0);
            rb_gc_register_mark_object(tp);
            rb_ary_push(tracepoints, tp);
        }
    }
}



VALUE lttng_module;

void Init_lttng_ust_ruby_agent()
{
    lttng_module = rb_define_module("LTTNG");

    rb_define_singleton_method(lttng_module, "debug",     ruby_lttng_trace_debug, 1);
    rb_define_singleton_method(lttng_module, "info",      ruby_lttng_trace_info, 1);
    rb_define_singleton_method(lttng_module, "notice",    ruby_lttng_trace_notice, 1);
    rb_define_singleton_method(lttng_module, "warning",   ruby_lttng_trace_warning, 1);
    rb_define_singleton_method(lttng_module, "err",       ruby_lttng_trace_err, 1);
    rb_define_singleton_method(lttng_module, "crit",      ruby_lttng_trace_crit, 1);
    rb_define_singleton_method(lttng_module, "alert",     ruby_lttng_trace_alert, 1);
    rb_define_singleton_method(lttng_module, "emerg",     ruby_lttng_trace_emerg, 1);

    rb_define_singleton_method(lttng_module, "trace_func_entry",    ruby_lttng_trace_func_entry, 4);
    rb_define_singleton_method(lttng_module, "trace_func_exit",     ruby_lttng_trace_func_exit, 4);
    rb_define_singleton_method(lttng_module, "trace_event",         ruby_lttng_trace_event, 6);

    rb_define_singleton_method(lttng_module, "register_tracepoints",  ruby_lttng_register_tracepoints, 0);
    rb_define_singleton_method(lttng_module, "enable_tracepoints",    ruby_lttng_enable_tracepoints, 0);
    rb_define_singleton_method(lttng_module, "disable_tracepoints",   ruby_lttng_disable_tracepoints, 0);

    init_gc_stat_sym_objs();
//    ruby_lttng_register_tracepoints(Qnil);
}

