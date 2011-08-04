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

if ($numArgs < 1) {
    print "This tool will search nic interrupts and set ";
    print "cpu affinity\n";
    print "Usage: set_interrupt.pl <eth1>\n";
    print "Example: set_interrupt.pl eth1\n";
    exit 1;
}

foreach $numArgs (0 .. $#ARGV) {
    print "$ARGV[$numArgs]\n";
}

my $nic = $ARGV[0];
my $command;
my $int_num;
my $nic_name;

foreach $_ (`cat /proc/interrupts | grep $nic`) {
    print "$_\n";
    ($int_num) = /(\d{1,3}:)/;
    ($nic_name) = /.*($nic.*)$/;
    print "device $nic_name is at interrupt $int_num\n";

    if ($nic_name =~ /usb4/) {
        print "we see usb4 at $nic_name\n"
    }

#    $fn = fn_trim($fn);
#    $command = "cp $src_dir/$fn $dest_dir/$fn\n";
#    print $command;
#    system($command);
}

