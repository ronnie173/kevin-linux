#!/bin/bash
# set netfilter parameters
echo "before set ip_conntrack_max"
cat /proc/sys/net/ipv4/netfilter/ip_conntrack_max

echo "1048576" > /proc/sys/net/ipv4/netfilter/ip_conntrack_max
echo "after set ip_conntrack_max"
cat /proc/sys/net/ipv4/netfilter/ip_conntrack_max

echo "before set ip_conntrack_tcp_timeout_established"
cat /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_timeout_established

echo "300" > /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_timeout_established
echo "after set ip_conntrack_tcp_timeout_established"
cat /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_timeout_established


