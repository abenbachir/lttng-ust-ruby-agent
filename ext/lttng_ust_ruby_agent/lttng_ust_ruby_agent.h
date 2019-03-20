#include <ruby.h>
#include <ruby/debug.h>
#include <ruby/debug.h>

/* To prevent unused parameter warnings */
#define UNUSED(x) (void)(x)
#define ARRAY_SIZE(array) sizeof(array)/sizeof(*array)

static int tracing_enabled = 0;
static VALUE tracepoints = Qnil;

static int gc_trace_events [] = {
    RUBY_INTERNAL_EVENT_GC_START,
    RUBY_INTERNAL_EVENT_GC_END_MARK,
    RUBY_INTERNAL_EVENT_GC_END_SWEEP,
    RUBY_INTERNAL_EVENT_NEWOBJ,
    RUBY_INTERNAL_EVENT_FREEOBJ,
#ifdef RUBY_INTERNAL_EVENT_GC_ENTER
    RUBY_INTERNAL_EVENT_GC_ENTER,
    RUBY_INTERNAL_EVENT_GC_EXIT
#endif
};
static int branch_trace_events [] = {
//    RUBY_EVENT_CALL,
//    RUBY_EVENT_RETURN,
//    RUBY_EVENT_C_CALL,
//    RUBY_EVENT_C_RETURN,
//    RUBY_EVENT_B_CALL,
//    RUBY_EVENT_B_RETURN,
//    RUBY_EVENT_LINE,
//    RUBY_EVENT_CLASS,
//    RUBY_EVENT_RAISE,
#ifdef RUBY_EVENT_SCRIPT_COMPILED
    RUBY_EVENT_SCRIPT_COMPILED,
#endif
    RUBY_EVENT_FIBER_SWITCH,
    RUBY_EVENT_THREAD_BEGIN,
    RUBY_EVENT_THREAD_END
};

struct gc_stat_sym_obj {
	char* name;
	VALUE sym;
	size_t (*get_value)(VALUE);
};
/* the order is important */
struct gc_stat_entry {
	size_t count;
	size_t heap_allocated_pages;
	size_t heap_sorted_length;
	size_t heap_allocatable_pages;
	size_t heap_available_slots;
	size_t heap_live_slots;
	size_t heap_free_slots;
    size_t heap_final_slots;
    size_t heap_marked_slots;
    size_t heap_eden_pages;
    size_t heap_tomb_pages;
    size_t total_allocated_pages;
    size_t total_freed_pages;
    size_t total_allocated_objects;
    size_t total_freed_objects;
    size_t malloc_increase_bytes;
    size_t malloc_increase_bytes_limit;
    size_t minor_gc_count;
    size_t major_gc_count;
    size_t remembered_wb_unprotected_objects;
    size_t remembered_wb_unprotected_objects_limit;
    size_t old_objects;
    size_t old_objects_limit;
    size_t oldmalloc_increase_bytes;
    size_t oldmalloc_increase_bytes_limit;
//    size_t major_by;
//    size_t gc_by;
//    size_t have_finalizer;
//    size_t immediate_sweep;
//    size_t state;
};

#define gc_stat_sym_init(stat_name, get_value_callback) {.name = #stat_name, .get_value = get_value_callback }

static struct gc_stat_sym_obj gc_stat_sym_objs[] = {
    /* sym_gc_stat */
	gc_stat_sym_init(count, rb_gc_stat),
	gc_stat_sym_init(heap_allocated_pages,                  rb_gc_stat),
	gc_stat_sym_init(heap_sorted_length,                    rb_gc_stat),
	gc_stat_sym_init(heap_allocatable_pages,                rb_gc_stat),
	gc_stat_sym_init(heap_available_slots,                  rb_gc_stat),
	gc_stat_sym_init(heap_live_slots,                       rb_gc_stat),
	gc_stat_sym_init(heap_free_slots,                       rb_gc_stat),
	gc_stat_sym_init(heap_final_slots,                      rb_gc_stat),
	gc_stat_sym_init(heap_marked_slots,                     rb_gc_stat),
	gc_stat_sym_init(heap_eden_pages,                       rb_gc_stat),
	gc_stat_sym_init(heap_tomb_pages,                       rb_gc_stat),
	gc_stat_sym_init(total_allocated_pages,                 rb_gc_stat),
	gc_stat_sym_init(total_freed_pages,                     rb_gc_stat),
	gc_stat_sym_init(total_allocated_objects,               rb_gc_stat),
	gc_stat_sym_init(total_freed_objects,                   rb_gc_stat),
	gc_stat_sym_init(malloc_increase_bytes,                 rb_gc_stat),
	gc_stat_sym_init(malloc_increase_bytes_limit,           rb_gc_stat),
	gc_stat_sym_init(minor_gc_count,                        rb_gc_stat),
	gc_stat_sym_init(major_gc_count,                        rb_gc_stat),
	gc_stat_sym_init(remembered_wb_unprotected_objects,     rb_gc_stat),
	gc_stat_sym_init(remembered_wb_unprotected_objects_limit, rb_gc_stat),
	gc_stat_sym_init(old_objects,                           rb_gc_stat),
	gc_stat_sym_init(old_objects_limit,                     rb_gc_stat),
	gc_stat_sym_init(oldmalloc_increase_bytes,              rb_gc_stat),
	gc_stat_sym_init(oldmalloc_increase_bytes_limit,        rb_gc_stat),
	/* sym_latest_gc_info */
	gc_stat_sym_init(major_by,                              rb_gc_latest_gc_info),
	gc_stat_sym_init(gc_by,                                 rb_gc_latest_gc_info),
	gc_stat_sym_init(have_finalizer,                        rb_gc_latest_gc_info),
	gc_stat_sym_init(immediate_sweep,                       rb_gc_latest_gc_info),
	gc_stat_sym_init(state,                                 rb_gc_latest_gc_info),
};

static void init_gc_stat_sym_objs(void)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(gc_stat_sym_objs); i++)
    {
        gc_stat_sym_objs[i].sym = ID2SYM(rb_intern(gc_stat_sym_objs[i].name));
    }
}

static struct gc_stat_entry get_gc_stat_entry(void)
{
    int i;
    struct gc_stat_entry gc_stat_entry = {};
    int size = sizeof(gc_stat_entry)/sizeof(size_t);
    size_t *buffer = (size_t *)&gc_stat_entry;
    for (i = 0; i < 2; i++)
    {
        buffer[i] = gc_stat_sym_objs[i].get_value(gc_stat_sym_objs[i].sym);
    }
    return gc_stat_entry;
}


static inline const char * obj_type(VALUE obj)
{
    switch (BUILTIN_TYPE(obj))
    {
#define CASE_TYPE(type) case T_##type: return #type
	CASE_TYPE(NONE);
	CASE_TYPE(NIL);
	CASE_TYPE(OBJECT);
	CASE_TYPE(CLASS);
	CASE_TYPE(ICLASS);
	CASE_TYPE(MODULE);
	CASE_TYPE(FLOAT);
	CASE_TYPE(STRING);
	CASE_TYPE(REGEXP);
	CASE_TYPE(ARRAY);
	CASE_TYPE(HASH);
	CASE_TYPE(STRUCT);
	CASE_TYPE(BIGNUM);
	CASE_TYPE(FILE);
	CASE_TYPE(FIXNUM);
	CASE_TYPE(TRUE);
	CASE_TYPE(FALSE);
	CASE_TYPE(DATA);
	CASE_TYPE(MATCH);
	CASE_TYPE(SYMBOL);
	CASE_TYPE(RATIONAL);
	CASE_TYPE(COMPLEX);
	CASE_TYPE(IMEMO);
	CASE_TYPE(UNDEF);
	CASE_TYPE(NODE);
	CASE_TYPE(ZOMBIE);
#undef CASE_TYPE
    }
    return "UNKNOWN";
}