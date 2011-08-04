#!/bin/bash
rmmod igb
sleep 2

modprobe igb InterruptThrottleRate=8000,8000 RSS=4,4 IntMode=2,2 QueuePairs=1,1
sleep 2

/etc/init.d/networking stop
sleep 5

/etc/init.d/networking start
sleep 5

cat /proc/interrupts

