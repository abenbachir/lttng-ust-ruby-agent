#!/usr/bin/env ruby
# require_relative "../../ext/lttng-ust-ruby-agent/lttng_module"
require_relative "version"
require "benchmark"

# This attempts to approximate a Logger-style API.
module LttngTracer
  # %i[ call return c_call c_return b_call b_return thread_begin thread_end raise end class line]
  DEFAULT_HOOKS = %i[call return raise thread_begin thread_end]
  C_CALL_HOOKS = %i[c_call c_return]
  LINE_HOOKS = %i[line]


  def trace(debug: false, follow_current_thread:false, trace_memory: false, trace_c_call: false, trace_line: false, max_graph_depth: Integer::MAX)
    # next if trace == false
    follow_thread = -1
    depth_per_thread = {}
    depth_per_thread[Thread.current.object_id] = 0

    hooks = DEFAULT_HOOKS
    hooks += C_CALL_HOOKS if trace_c_call
    hooks += LINE_HOOKS if trace_line
    # p hooks

    follow_thread = Thread.current.object_id if follow_current_thread

    trace = TracePoint.new(*hooks) { |tp|
      begin
        next if follow_thread > 0 and follow_thread == Thread.current.object_id

        defined_class = tp.defined_class.inspect
        # skip tracing when we are inside TracePoint or Lttng

        if tp.event == :thread_end
          rss_kb = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0
          LTTNG.trace_event(tp.lineno, rss_kb, tp.event.to_s, "", "", "")
          next
        elsif tp.event == :thread_begin
          rss_kb = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0
          LTTNG.trace_event(tp.lineno, rss_kb, tp.event.to_s, "", "", "")
          depth_per_thread[Thread.current.object_id] = 0
          next
        elsif tp.event == :raise
          rss_kb = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0
          LTTNG.trace_event(tp.lineno, rss_kb, tp.event.to_s, "", "", "")
          next
        end

        next if defined_class.include? "TracePoint" or defined_class.include? "LttngTracer" \
          or defined_class.include? "LTTNG" or tp.path.include? __FILE__

        if depth_per_thread[Thread.current.object_id].nil?
          depth_per_thread[Thread.current.object_id] = 0
        end

        next if tp.event == :return and depth_per_thread[Thread.current.object_id] == 0

        # update current depth
        if [:call, :c_call].include?(tp.event)
          depth_per_thread[Thread.current.object_id] += 1
        end

        current_depth = depth_per_thread[Thread.current.object_id]

        if current_depth <= max_graph_depth

          method = tp.method_id.to_s
          rss_kb = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0

          case tp.event
          when :call, :c_call
            func_name = defined_class << ':' << method
            LTTNG.trace_func_entry(tp.method_id.object_id, func_name, GC.stat[:heap_live_slots], rss_kb)
            puts "entry | %d | %d | %s%s() [%d KB] %s" % [Thread.current.object_id, current_depth, " - " * current_depth, func_name, rss_kb, tp.path]  if debug
          when :return, :c_return
            func_name = defined_class << ':' << method
            LTTNG.trace_func_exit(tp.method_id.object_id, func_name, GC.stat[:heap_live_slots], rss_kb)
            puts "exit  | %d | %d | %s} /*%s [%d KB]*/ %s" % [Thread.current.object_id, current_depth, " - " * current_depth, func_name, rss_kb, tp.path] if debug
          else
            LTTNG.trace_event(tp.lineno, rss_kb, tp.event.to_s, defined_class, method, tp.path)
          end
        end
        # update current depth
        if [:return, :c_return].include?(tp.event)
          depth_per_thread[Thread.current.object_id] -= 1
        end
      rescue Exception => e
        puts "LttngTracer:trace exception : "  + e.to_s
        p e.backtrace
        p [tp.defined_class.inspect + ':' + tp.method_id.to_s, tp.event.to_s, Thread.current.object_id, tp.path]
        puts "\n"
      end
    }

    trace.enable
    yield
    trace.disable
  end
  module_function :trace

  def trace_block(event_name:, trace_memory: true, debug: false)
    begin
      rss_kb_before = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0
      gc_stat_before = GC.stat[:count]
      LTTNG.trace_func_entry(event_name.object_id, event_name, gc_stat_before, rss_kb_before)
      puts "start_block: | %d | %s [%d KB]" % [Thread.current.object_id, event_name, rss_kb_before] if debug
    rescue Exception => e
      puts "LttngTracer:trace_block exception : "  + e.to_s
      p e.backtrace
    end

    yield

    begin
      gc_stat_after = GC.stat[:count]
      rss_kb_after = trace_memory ? `ps -o rss= -p #{Process.pid}`.to_i : 0
      LTTNG.trace_func_exit(event_name.object_id, event_name, gc_stat_after, rss_kb_after)

      puts "end_block:  | %d | %s [%d KB / diff %d KB]" % [Thread.current.object_id,event_name, rss_kb_after, rss_kb_after-rss_kb_before] if debug
    rescue Exception => e
      puts "LttngTracer:trace_block exception : "  + e.to_s
      p e.backtrace
    end
  end
  module_function :trace_block

  def measure_debug(no_gc = false, &block)

    if no_gc
      GC.disable
    else
      # collect memory allocated during library loading
      # and our own code before the measurement
      GC.start
    end

    memory_before = `ps -o rss= -p #{Process.pid}`.to_i
    gc_stat_before = GC.stat
    time = Benchmark.realtime do
      yield
    end
    # puts ObjectSpace.count_objects
    unless no_gc
      GC.start(full_mark: true, immediate_sweep: true, immediate_mark: false)
    end
    # puts ObjectSpace.count_objects
    gc_stat_after = GC.stat
    memory_after = `ps -o rss= -p #{Process.pid}`.to_i
    # LTTNG.trace_event(tp.lineno, rss_kb, tp.event.to_s, defined_class, method, tp.path)
    return {
      time_us: (time * 10**6).round(2),
      ruby_version: RUBY_VERSION,
      gc: no_gc ? 'disabled' : 'enabled',
      gc_invokation: gc_stat_after[:count] - gc_stat_before[:count],
      rss_kb: memory_after - memory_before
    }
  end

end

