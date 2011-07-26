#!/bin/bash

# Parameter settings
  EXTIF="eth1"           # To public interface
  EXTIF_IRQ="16"         # To public interface irq
  INIF="eth2"            # To internal interface
  INIF_IRQ="17"          # To internal interface irq
  export EXTIF INIF EXTIF_IRQ INIF_IRQ

# Part one, set smp affinity
# let cpu0 and cpu1 handle extif irq
echo "3" > /proc/irq/$EXTIF_IRQ/smp_affinity
echo "/proc/irq/$EXTIF_IRQ/smp_affinity"
cat /proc/irq/$EXTIF_IRQ/smp_affinity

# let cpu2 and cpu3 handle inif irq
echo "c" > /proc/irq/$INIF_IRQ/smp_affinity
echo "/proc/irq/$INIF_IRQ/smp_affinity"
cat /proc/irq/$INIF_IRQ/smp_affinity
