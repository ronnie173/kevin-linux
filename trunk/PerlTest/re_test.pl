#!/usr/bin/perl -W
#
# This is a regular expression test.
#
# Authors: zhangjw@fortinet.com
#
###########################################

use strict;
use warnings;

sub main {
    my $tmp = "FG43443 login:";
    my $result = $tmp =~ m/.*login:/;
    
    print $result . "\n";
    
    return 0;
}

main();

