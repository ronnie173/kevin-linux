#!/usr/bin/perl -w

use strict;
use warnings;

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
    print "This tool will search all *.c and *.h in ";
    print "current directory and copy the same files ";
    print " from source directory to replace current files\n";
    print "Usage: cp_same_files.pl <src_dir>\n";
    print "Example: del_ip.pl ../my_src_code\n";
    exit 1;
}

foreach $numArgs (0 .. $#ARGV) {
    print "$ARGV[$numArgs]\n";
}

my $src_dir = $ARGV[0];
my $dest_dir = ".";
my @files = `find $dest_dir -name "*.[c|h]"`;
my $fn;
my $command;

foreach $fn (@files) {
    chomp($fn);
    $fn = fn_trim($fn);
    $command = "cp $src_dir/$fn $dest_dir/$fn\n";
    print $command;
    system($command);
}

