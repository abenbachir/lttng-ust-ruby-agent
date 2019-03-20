#!/usr/bin/env ruby

# ObjectSpace.trace_object_allocations_start
# begin
#   gem "lttng-ust-ruby-agent"
#   require "lttng-ust-ruby-agent"
#   LTTNG.enable_tracepoints()
# rescue Gem::LoadError
#   puts "Warning: GEM lttng-ust-ruby-agent not installed, tracing will not be available"
# end


def func_exception
  test = 324 / 5
end

def func_c(data)
  data.map! { |str| str.upcase }
  func_exception
end

def func_b(length)
  data = Array.new(length, "x")

  func_c(data)

end

def func_a
  puts "func_a()"
  length = 10000
  func_b(length)

end



def func_csv_not_optimized
  num_rows = 10000
  num_cols = 10
  # LTTNG.info("begin: func_csv_not_optimized()")
  data = Array.new(num_rows) { Array.new(num_cols) { "x" * 1000 }  }
  csv = data.map { |row| row.join(",") }.join("\n")
  File.open('./tmp/not_optimized-%d.csv' % [Thread.current.object_id],'w') do |f|
    f.write(csv)
  end
  # LTTNG.info("end: func_csv_not_optimized()")
end

def func_csv_optimized
  num_rows = 10000
  num_cols = 10
  data = Array.new(num_rows) { Array.new(num_cols) { "x" * 1000 }  }
  csv = ''
  num_rows.times do |i|
    num_cols.times do |j|
      csv << data[i][j]
      csv << "," unless j == (num_cols -1)
    end
    csv << "\n" unless i == (num_rows -1)
  end
end


# LttngTracer.trace(trace_memory: true, trace_c_call: true, max_graph_depth:2) do
#   func_a
# end
#
# LttngTracer.trace(trace_memory: true, trace_c_call: true, max_graph_depth: 2) do
#   func_csv_not_optimized
#   func_csv_optimized
# end

myvar = '{"msg":"1"}'
e = 'HTTP error: (request-id=; class=Net::HTTPBadRequest; code=400; message=The specified block list is invalid.; body=﻿<?xml version="1.0" encoding="utf-8"?><Error><Code>InvalidBlockList</Code><Message>The specified block list is invalid.
RequestId:dcd2983f-001e-007f-06ec-ca684d000000'
# puts "#{myvar}".to_json
p "test: " << %Q('#{e}')
p %(Hello "world"!)
puts "Started At #{Time.now}"
t1 = Thread.new{func_csv_not_optimized()}
t2 = Thread.new{func_csv_not_optimized()}
t3 = Thread.new{func_csv_not_optimized()}

puts "Join at #{Time.now}"
t1.join
t2.join
t3.join
puts "End at #{Time.now}"

# File.open('heap_dump.json','w') do |f|
#   ObjectSpace.dump_all(output: f)
# end
#
