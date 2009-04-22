#!/usr/bin/perl -w

use strict;

my $numArgs = $#ARGV + 1;
print "I see $numArgs arguments.\n";

if ($numArgs < 5) {
    print "del_ip.pl ip_prefix ip_start ip_end ip_mask dev\n";
    print "del_ip.pl 192.168.1. 114 124 24 eth0\n";
    exit 1;
}

foreach $numArgs (0 .. $#ARGV) {
    print "$ARGV[$numArgs]\n";
}

my $prefix = $ARGV[0];
my $start = $ARGV[1];
my $end = $ARGV[2];
my $mask = $ARGV[3];
my $dev = $ARGV[4];

my $addr;
my $ipaddr;

foreach $addr ($start .. $end) {
    my $ipaddr = "ip addr delete $prefix$addr/$mask dev $dev";
    print "$ipaddr\n";
    system($ipaddr);
}

