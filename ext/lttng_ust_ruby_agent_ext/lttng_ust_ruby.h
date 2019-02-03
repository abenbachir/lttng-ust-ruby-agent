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

#endif /* _TRACEPOINT_LTTNG_UST_RUBY_H */


#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./lttng_ust_ruby.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

