# struct rb_compile_option_struct {
#      unsigned int inline_const_cache: 1;
#      unsigned int peephole_optimization: 1;
#      unsigned int tailcall_optimization: 1;
#      unsigned int specialized_instruction: 1;
#      unsigned int operands_unification: 1;
#      unsigned int instructions_unification: 1;
#      unsigned int stack_caching: 1;
#      unsigned int trace_instruction: 1;
#      unsigned int frozen_string_literal: 1;
#      unsigned int debug_frozen_string_literal: 1;
#      unsigned int coverage_enabled: 1;
#      int debug_level;
#  };
# 334 static rb_compile_option_t COMPILE_OPTION_DEFAULT = {
# 335     OPT_INLINE_CONST_CACHE, /* int inline_const_cache; */
# 336     OPT_PEEPHOLE_OPTIMIZATION, /* int peephole_optimization; */
# 337     OPT_TAILCALL_OPTIMIZATION, /* int tailcall_optimization */
# 338     OPT_SPECIALISED_INSTRUCTION, /* int specialized_instruction; */
# 339     OPT_OPERANDS_UNIFICATION, /* int operands_unification; */
# 340     OPT_INSTRUCTIONS_UNIFICATION, /* int instructions_unification; */
# 341     OPT_STACK_CACHING, /* int stack_caching; */
# 342     OPT_TRACE_INSTRUCTION, /* int trace_instruction */
# 343     OPT_FROZEN_STRING_LITERAL,
# 344     OPT_DEBUG_FROZEN_STRING_LITERAL,
# 345     TRUE,           /* coverage_enabled */
# 346 };

options = {
    # tailcall_optimization: true,
    trace_instruction: false,
    # instructions_unification: true,
    # stack_caching: true,
    debug_level: -1,
    coverage_enabled: false,
    # frozen_string_literal: true
}

def bench(itr = 1) # :yield:
  times = []
  for i in 1..itr
    t0 = Time.now
    yield
    t1 = Time.now
    times << t1 - t0
  end
  times
end

def median(array)
  sorted = array.sort
  len = sorted.length
  (sorted[(len - 1) / 2] + sorted[len / 2]) / 2.0
end
def stats(arr)
  round = 5
  lowest = arr.min.round(round)
  highest = arr.max.round(round)
  total = arr.inject(:+)
  len = arr.length
  average = (total.to_f / len).round(round) # to_f so we don't get an integer result
  median = median(arr).round(round)
  return { avg:average, median:median, min:lowest, max:highest}
end
small_code = <<-EOS
# $HASH = { "key1": 123, "key2": 456 }
def emit()
    a = 1
    b = 2
    c = 3
    a*b*c
end
for i in 0..1000000
  emit
end
EOS
factorial_code = <<-EOS
class Factorial
  def fact_helper(n, res)
    n == 1 ? res : fact_helper(n - 1, n * res)
  end

  def fact(n)
    fact_helper(n, 1)
  end
end

Factorial.new.fact(10000)
EOS

iterations = 100
some_code = small_code
GC.disable
not_optimized_method = RubyVM::InstructionSequence.compile(some_code)
puts RubyVM::InstructionSequence.compile(some_code).disasm
times = bench(iterations) { not_optimized_method.eval }
no_opt_stats = stats(times)
p "No optimization:", no_opt_stats

RubyVM::InstructionSequence.compile_option = options
optimized_method = RubyVM::InstructionSequence.compile(some_code)
puts RubyVM::InstructionSequence.compile(some_code).disasm
times = bench(iterations) { optimized_method.eval }
opt_stats = stats(times)
p "With optimization:", opt_stats

