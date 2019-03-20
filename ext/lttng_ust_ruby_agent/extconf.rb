require 'mkmf'
have_library("lttng-ust")
create_makefile('lttng_ust_ruby_agent')

