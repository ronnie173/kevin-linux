#!/bin/bash

# Parameter settings
  ETH1_IRQ="44"           # To public interface
  ETH1_RX0_IRQ="45"         # To public interface irq
  ETH2_IRQ="53"            # To internal interface
  ETH2_RX0_IRQ="55"          # To internal interface irq
  export ETH1_IRQ ETH2_IRQ ETH1_RX0_IRQ ETH2_RX0_IRQ

# Part one, set smp affinity
echo "f" > /proc/irq/44/smp_affinity
echo "1" > /proc/irq/45/smp_affinity
echo "2" > /proc/irq/46/smp_affinity
echo "4" > /proc/irq/47/smp_affinity
echo "8" > /proc/irq/48/smp_affinity
echo "1" > /proc/irq/49/smp_affinity
echo "2" > /proc/irq/50/smp_affinity
echo "4" > /proc/irq/51/smp_affinity
echo "8" > /proc/irq/52/smp_affinity

cat /proc/irq/44/smp_affinity
cat /proc/irq/45/smp_affinity
cat /proc/irq/46/smp_affinity
cat /proc/irq/47/smp_affinity
cat /proc/irq/48/smp_affinity
cat /proc/irq/49/smp_affinity
cat /proc/irq/50/smp_affinity
cat /proc/irq/51/smp_affinity
cat /proc/irq/52/smp_affinity

echo "f" > /proc/irq/53/smp_affinity
echo "1" > /proc/irq/55/smp_affinity
echo "2" > /proc/irq/56/smp_affinity
echo "4" > /proc/irq/57/smp_affinity
echo "8" > /proc/irq/58/smp_affinity
echo "1" > /proc/irq/59/smp_affinity
echo "2" > /proc/irq/60/smp_affinity
echo "4" > /proc/irq/61/smp_affinity
echo "8" > /proc/irq/62/smp_affinity

cat /proc/irq/53/smp_affinity
cat /proc/irq/55/smp_affinity
cat /proc/irq/56/smp_affinity
cat /proc/irq/57/smp_affinity
cat /proc/irq/58/smp_affinity
cat /proc/irq/59/smp_affinity
cat /proc/irq/60/smp_affinity
cat /proc/irq/61/smp_affinity
cat /proc/irq/62/smp_affinity

