#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER lttng_ust_cyg_profile

#if !defined(_TRACEPOINT_LTTNG_UST_CYG_PROFILE_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_CYG_PROFILE_H

#include "ruby.h"
#include <lttng/tracepoint.h>


TRACEPOINT_EVENT_CLASS(lttng_ust_cyg_profile, func_class,
	TP_ARGS(unsigned long, func_addr, char*, name, unsigned long int, gc_stat_count, unsigned long int, rss_kb),
	TP_FIELDS(
		ctf_integer_hex(unsigned long, addr, (unsigned long) func_addr)
		ctf_string(name, name)
		ctf_integer(unsigned long int, gc_stat_count, (unsigned long int) gc_stat_count)
		ctf_integer(unsigned long int, rss_kb, rss_kb)
	)
)

TRACEPOINT_EVENT_INSTANCE(lttng_ust_cyg_profile, func_class,
	func_entry,
	TP_ARGS(unsigned long, func_addr, char*, name, unsigned long int, gc_stat_count, unsigned long int, rss_kb)
)

//TRACEPOINT_LOGLEVEL(lttng_ust_cyg_profile, func_entry,
//	TRACE_DEBUG_FUNCTION)

TRACEPOINT_EVENT_INSTANCE(lttng_ust_cyg_profile, func_class,
	func_exit,
	TP_ARGS(unsigned long, func_addr, char*, name, unsigned long int, gc_stat_count, unsigned long int, rss_kb)
)

//TRACEPOINT_LOGLEVEL(lttng_ust_cyg_profile, func_exit,
//	TRACE_DEBUG_FUNCTION)

#endif /* _TRACEPOINT_LTTNG_UST_CYG_PROFILE_H */


#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./lttng_ust_cyg_profile.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

