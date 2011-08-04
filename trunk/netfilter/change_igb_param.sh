#!/bin/bash
echo "remove module igb"
rmmod igb
sleep 2

echo "list modules after rmmod"
lsmod | grep igb
sleep 1

modprobe igb InterruptThrottleRate=5000,5000 RSS=0,0 IntMode=2,2 QueuePairs=1,1
sleep 2

echo "list modules after modprobe"
lsmod | grep igb
sleep 1

echo "stop network service"
/etc/init.d/networking stop
sleep 5

echo "start network service"
/etc/init.d/networking start
sleep 5

cat /proc/interrupts

