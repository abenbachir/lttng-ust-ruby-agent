#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER lttng_ruby_log

#if !defined(_TRACEPOINT_LTTNG_UST_RUBY_LOG) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_RUBY_LOG

#include "ruby.h"
#include <lttng/tracepoint.h>



TRACEPOINT_EVENT(lttng_ruby_log,
                 debug,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, debug, TRACE_DEBUG)



TRACEPOINT_EVENT(lttng_ruby_log,
                 info,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, info, TRACE_INFO)



TRACEPOINT_EVENT(lttng_ruby_log,
                 notice,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, notice, TRACE_NOTICE)



TRACEPOINT_EVENT(lttng_ruby_log,
                 warning,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, warning, TRACE_WARNING)



TRACEPOINT_EVENT(lttng_ruby_log,
                 err,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, err, TRACE_ERR)



TRACEPOINT_EVENT(lttng_ruby_log,
                 crit,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, crit, TRACE_CRIT)



TRACEPOINT_EVENT(lttng_ruby_log,
                 alert,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, alert, TRACE_ALERT)



TRACEPOINT_EVENT(lttng_ruby_log,
                 emerg,
                 TP_ARGS(const char*, t_sourcefile,
                         int, t_sourceline,
                         char*, t_message),
                 TP_FIELDS(ctf_string(sourcefile, t_sourcefile)
                           ctf_integer(int, sourceline, t_sourceline)
                           ctf_string(message, t_message)))
TRACEPOINT_LOGLEVEL(lttng_ruby_log, emerg, TRACE_EMERG)


#endif /* _TRACEPOINT_LTTNG_UST_RUBY_LOG */

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./lttng_ust_ruby_log.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

