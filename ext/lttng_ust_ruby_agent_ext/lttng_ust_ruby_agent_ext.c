#include <ruby.h>

#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE
#include "lttng_ust_ruby_log.h"
#include "lttng_ust_cyg_profile.h"
#include "lttng_ust_ruby.h"


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

VALUE lttng_module;

void Init_lttng_ust_ruby_agent_ext()
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
}

