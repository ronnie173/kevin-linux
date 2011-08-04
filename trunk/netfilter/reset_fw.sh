#!/bin/bash

# Parameter settings
  EXTIF="eth1"           # To public interface
  INIF="eth2"            # To internal interface
  INNET="10.11.0.0/16" # Internal subnet
  export EXTIF INIF INNET

# Part one, firewall unit settings
# 1. kernel network settings
#  echo "1" > /proc/sys/net/ipv4/tcp_syncookies
#  echo "1" > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
#  for i in /proc/sys/net/ipv4/conf/*/{rp_filter,log_martians}; do
#        echo "1" > $i
#  done
#  for i in /proc/sys/net/ipv4/conf/*/{,accept_source_route,accept_redirects,\
#send_redirects}; do
#        echo "0" > $i
#  done

# 2. clear rule, predefine policy and lo related settings
  PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/sbin:/usr/local/bin; export PATH
  iptables -F

