#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER lttng_ruby

#if !defined(_TRACEPOINT_LTTNG_UST_RUBY_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_RUBY_H

#include "ruby.h"
#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(lttng_ruby, event,
	TP_ARGS(
		unsigned int, lineno,
		long unsigned int, rss_kb,
		const char *, event,
		const char *, class_name,
		const char *, func_name,
		const char *, path
	),
	TP_FIELDS(
		ctf_integer(unsigned int, lineno, lineno)
		ctf_integer(long unsigned int, rss_kb, rss_kb)
		ctf_string(event, event)
		ctf_string(class_name, class_name)
		ctf_string(func_name, func_name)
		ctf_string(path, path)
	)
)

TRACEPOINT_EVENT_CLASS(lttng_ruby, lttng_ruby_empty_class,
	TP_ARGS(), TP_FIELDS()
)

TRACEPOINT_EVENT_CLASS(lttng_ruby, lttng_ruby_gc_stat_heap_class,
	TP_ARGS(struct gc_stat_entry*, entry),
	TP_FIELDS(
		ctf_integer(unsigned int, count, entry->count)
        ctf_integer(unsigned int, heap_allocated_pages, entry->heap_allocated_pages)
        ctf_integer(unsigned int, heap_sorted_length, entry->heap_sorted_length)
        ctf_integer(unsigned int, heap_allocatable_pages, entry->heap_allocatable_pages)
        ctf_integer(unsigned int, heap_available_slots, entry->heap_available_slots)
        ctf_integer(unsigned int, heap_live_slots, entry->heap_live_slots)
        ctf_integer(unsigned int, heap_free_slots, entry->heap_free_slots)
        ctf_integer(unsigned int, heap_final_slots, entry->heap_final_slots)
        ctf_integer(unsigned int, heap_marked_slots, entry->heap_marked_slots)
        ctf_integer(unsigned int, heap_eden_pages, entry->heap_eden_pages)
        ctf_integer(unsigned int, total_allocated_pages, entry->total_allocated_pages)
        ctf_integer(unsigned int, total_freed_pages, entry->total_freed_pages)
        ctf_integer(unsigned int, total_allocated_objects, entry->total_allocated_objects)
        ctf_integer(unsigned int, total_freed_objects, entry->total_freed_objects)
        ctf_integer(unsigned int, malloc_increase_bytes, entry->malloc_increase_bytes)
        ctf_integer(unsigned int, malloc_increase_bytes_limit, entry->malloc_increase_bytes_limit)
        ctf_integer(unsigned int, minor_gc_count, entry->minor_gc_count)
        ctf_integer(unsigned int, major_gc_count, entry->major_gc_count)
        ctf_integer(unsigned int, remembered_wb_unprotected_objects, entry->remembered_wb_unprotected_objects)
        ctf_integer(unsigned int, remembered_wb_unprotected_objects_limit, entry->remembered_wb_unprotected_objects_limit)
        ctf_integer(unsigned int, old_objects, entry->old_objects)
        ctf_integer(unsigned int, old_objects_limit, entry->old_objects_limit)
        ctf_integer(unsigned int, oldmalloc_increase_bytes, entry->oldmalloc_increase_bytes)
        ctf_integer(unsigned int, oldmalloc_increase_bytes_limit, entry->oldmalloc_increase_bytes_limit)
	)
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_gc_stat_heap_class, gc_stat,
	TP_ARGS(struct gc_stat_entry*, entry)
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, gc_start,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, gc_end_mark,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, gc_end_sweep,
	TP_ARGS()
)

TRACEPOINT_EVENT(lttng_ruby, newobj,
	TP_ARGS(
		long unsigned int, obj,
		unsigned int, flags,
		unsigned int, lineno,
		const char*, type,
		const char*, klass,
		const char*, path,
		const char*, method
	),
	TP_FIELDS(
		ctf_integer_hex(long unsigned int, obj, obj)
		ctf_integer_hex(unsigned int, flags, flags)
		ctf_integer(unsigned int, lineno, lineno)
		ctf_string(type, type)
		ctf_string(klass, klass)
		ctf_string(path, path)
		ctf_string(method, method)
	)
)

TRACEPOINT_EVENT(lttng_ruby, freeobj,
	TP_ARGS(
		long unsigned int, obj,
		unsigned int, flags,
		const char*, type
	),
	TP_FIELDS(
		ctf_integer_hex(long unsigned int, obj, obj)
		ctf_integer_hex(unsigned int, flags, flags)
		ctf_string(type, type)
	)
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, gc_enter,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, gc_exit,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, thread_begin,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, thread_end,
	TP_ARGS()
)

TRACEPOINT_EVENT_INSTANCE(lttng_ruby, lttng_ruby_empty_class, fiber_switch,
	TP_ARGS()
)

#endif /* _TRACEPOINT_LTTNG_UST_RUBY_H */


#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./lttng_provider/lttng_ust_ruby.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

