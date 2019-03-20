#!/usr/bin/env bash
output=/home/${USER}/lttng-traces/ruby-trace

sudo gem uninstall lttng-ust-ruby-agent
sudo gem install lttng-ust-ruby-agent-0.1.gem

rm -rf ${output}
lttng create rubytrace --output="$output"

lttng enable-channel -u --subbuf-size 1024K --num-subbuf 128 ust_mychannel
lttng enable-channel -k --subbuf-size 1024K --num-subbuf 128 k_channel
lttng enable-event -k "sched_*,lttng_statedump*,timer*,irq*, block_rq_complete"  -c k_channel
lttng enable-event -k "block_*"  -c k_channel
lttng enable-event -k --syscall -a -c k_channel

lttng enable-event -u 'lttng_ust_cyg_profile*,lttng_ruby*' -c ust_mychannel
lttng add-context -u -t vpid -t vtid -t procname -c ust_mychannel

lttng start
./bin/lttng-ruby ./demos/demo.rb

lttng stop
#mkdir -p ${output}/mapping/
#lttng view | grep func_entry | python dump-symbols.py > ${output}/mapping/symbols.sym

lttng destroy
