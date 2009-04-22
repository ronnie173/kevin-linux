#!/usr/bin/perl -w

use strict;

# Declare the subroutines
sub trim($);
sub ltrim($);
sub rtrim($);

# Perl trim function to remove whitespace from the start and end of the string
sub trim($) {
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}

# Left trim function to remove leading whitespace
sub ltrim($) {
    my $string = shift;
    $string =~ s/^\s+//;
    return $string;
}

# Right trim function to remove trailing whitespace
sub rtrim($) {
    my $string = shift;
    $string =~ s/\s+$//;
    return $string;
}

my $numArgs = $#ARGV + 1;
print "I see $numArgs arguments.\n";

if ($numArgs < 1) {
    print "Usage: del_ip.pl <dev>\n";
    print "Example: del_ip.pl eth0\n";
    exit 1;
}

foreach $numArgs (0 .. $#ARGV) {
    print "$ARGV[$numArgs]\n";
}

my $dev = $ARGV[0];
my $command;
my $ipaddr;
my $rest;

foreach $_ (`ip addr show dev $dev | grep secondary`) {
    print "$_\n";
    ($ipaddr) = /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\/24)/;
    print "ipaddr=[$ipaddr]\n";
    
    if (length(trim($ipaddr)) > 0) {
        $command = "ip addr delete $ipaddr dev $dev";
        print "command=[$command]\n";
        system($command);
    }
}

my $result = `ip addr show dev $dev`;
print $result;
