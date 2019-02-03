module LTTNG
  VERSION = "0.1.0"
  VERSION_MAJOR = 0
  VERSION_MINOR = 1
  VERSION_TINY = 0
end


class Integer
  N_BYTES = [42].pack('i').size
  N_BITS = N_BYTES * 16
  MAX = 2 ** (N_BITS - 2) - 1
  MIN = -MAX - 1
end