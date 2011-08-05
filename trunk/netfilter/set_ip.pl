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

# trim starting ". or /"
sub fn_trim($) {
    my $str = shift;
    my $ind;
    my $char;
    my @str_array = split(//, $str);
    my $len = scalar @str_array;
    for ($ind = 0; $ind < $len; $ind++) {
        $char = $str_array[$ind];
        if ($char ne '.' && $char ne '/') {
            last;
        }
    }

    $str = substr($str, $ind);
    return $str;
}

my $numArgs = $#ARGV + 1;
print "I see $numArgs arguments.\n";

if ($numArgs < 5) {
    print "This tool will add N ip of a subnet to a NIC\n";
    print "Usage: set_ip.pl <subnet> <netmask> <start> <end> <dev>\n";
    print "Example: set_ip.pl 10.21.13. 16 1 200 eth4\n";
    exit 1;
}

foreach $numArgs (0 .. $#ARGV) {
    print "$ARGV[$numArgs]\n";
}

my $subnet = $ARGV[0];
my $netmask = $ARGV[1];
my $start = $ARGV[2];
my $end = $ARGV[3];
my $nic = $ARGV[4];
my $command;
my $i;

for ($i = $start; $i <= $end; $i++) {
    $command = "ip addr add $subnet$i/$netmask dev $nic";
    print "$command\n";
    system($command);
}

