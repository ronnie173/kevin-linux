#!/bin/bash
rmmod igb
modprobe igb InterruptThrottleRate=8000,8000 RSS=4,4 IntMode=2,2 QueuePairs=0,0
/etc/init.d/networking stop
/etc/init.d/networking start

