#!/bin/bash

# Parameter settings
  EXTIF="eth0"           # To public interface
  INIF="eth2"            # To internal interface
  export EXTIF INIF

# Part one, set smp affinity
# let cpu0 and cpu1 handle extif irq
echo "0" > /sys/class/net/$EXTIF/queues/rx-0/rps_cpus
echo "0" > /sys/class/net/$EXTIF/queues/tx-0/xps_cpus

# let cpu2 and cpu3 handle inif irq
echo "0" > /sys/class/net/$INIF/queues/rx-0/rps_cpus
echo "0" > /sys/class/net/$INIF/queues/tx-0/xps_cpus

