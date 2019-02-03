#!/usr/bin/env bash
output=/home/${USER}/lttng-traces/ruby-trace


rm -rf ${output}
lttng create rubytrace --output="$output"

lttng enable-channel -u --subbuf-size 1024K --num-subbuf 128 ust_mychannel
lttng enable-channel -k --subbuf-size 1024K --num-subbuf 128 k_channel
lttng enable-event -k "sched_switch"  -c k_channel

lttng enable-event -u 'lttng_ust_cyg_profile*,lttng_ruby*' -c ust_mychannel
lttng add-context -u -t vpid -t vtid -t procname -c ust_mychannel

lttng start
ruby ./demos/demo.rb

lttng stop
mkdir -p ${output}/mapping/
lttng view | grep func_entry | python dump-symbols.py > ${output}/mapping/symbols.sym

lttng destroy
