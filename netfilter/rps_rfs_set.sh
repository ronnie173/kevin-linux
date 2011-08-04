#!/bin/bash

# Parameter settings
  EXTIF="eth1"           # To public interface
  INIF="eth2"            # To internal interface
  export EXTIF INIF

# Part one, set smp affinity
# let cpu0 and cpu1 handle extif irq
echo "3" > /sys/class/net/$EXTIF/queues/rx-0/rps_cpus
echo "c" > /sys/class/net/$EXTIF/queues/tx-0/xps_cpus
#echo "f" > /sys/class/net/$EXTIF/queues/rx-0/rps_cpus
#echo "f" > /sys/class/net/$EXTIF/queues/tx-0/xps_cpus
echo "4096" > /sys/class/net/$EXTIF/queues/rx-0/rps_flow_cnt

echo "$EXTIF rx-0/rps_cpus tx-0/xps_cpus rx-0/rps_flow_cnt"
cat /sys/class/net/$EXTIF/queues/rx-0/rps_cpus
cat /sys/class/net/$EXTIF/queues/tx-0/xps_cpus
cat /sys/class/net/$EXTIF/queues/rx-0/rps_flow_cnt

# let cpu2 and cpu3 handle inif irq
echo "c" > /sys/class/net/$INIF/queues/rx-0/rps_cpus
echo "3" > /sys/class/net/$INIF/queues/tx-0/xps_cpus
#echo "f" > /sys/class/net/$INIF/queues/rx-0/rps_cpus
#echo "f" > /sys/class/net/$INIF/queues/tx-0/xps_cpus
echo "4096" > /sys/class/net/$INIF/queues/rx-0/rps_flow_cnt

echo "$INIF rx-0/rps_cpus tx-0/xps_cpus rx-0/rps_flow_cnt"
cat /sys/class/net/$INIF/queues/rx-0/rps_cpus
cat /sys/class/net/$INIF/queues/tx-0/xps_cpus
cat /sys/class/net/$INIF/queues/rx-0/rps_flow_cnt

# set /proc/sys/net/core
echo "4096" > /proc/sys/net/core/rps_sock_flow_entries
echo "rps_sock_flow_entries"
cat /proc/sys/net/core/rps_sock_flow_entries


