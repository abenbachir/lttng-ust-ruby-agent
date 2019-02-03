begin
  gem "lttng-ust-ruby-agent"
  require "lttng-ust-ruby-agent"
rescue Gem::LoadError
  puts "Warning: GEM lttng-ust-ruby-agent not installed, tracing will not be available"
  module LttngTracer
    def trace(*args); end; module_function :trace
    def trace_block(*args); end; module_function :trace_block
  end
end


def func_exception
  test = 324 / 0
end

def func_c(data)
  LttngTracer.trace_block(event_name: "data.map!{|str|str.upcase}") do
    data.map! { |str| str.upcase }
  end
  func_exception
end

def func_b(length)
  data = Array.new(length, "x")

  func_c(data)

end

def func_a
  length = 10000
  func_b(length)

end

$num_rows = 10000
$num_cols = 10

def func_csv_not_optimized
  data = Array.new($num_rows) { Array.new($num_cols) { "x" * 1000 }  }
  csv = data.map { |row| row.join(",") }.join("\n")
end

def func_csv_optimized
  data = Array.new($num_rows) { Array.new($num_cols) { "x" * 1000 }  }
  csv = ''
  $num_rows.times do |i|
    $num_cols.times do |j|
      csv << data[i][j]
      csv << "," unless j == ($num_cols -1)
    end
    csv << "\n" unless i == ($num_rows -1)
  end
end


LttngTracer.trace(trace_memory: true, trace_c_call: true, max_graph_depth:2) do
  func_a
end

LttngTracer.trace(trace_memory: true, trace_c_call: true, max_graph_depth: 2) do
  func_csv_not_optimized
  func_csv_optimized
end

