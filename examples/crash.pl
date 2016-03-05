#!/usr/bin/perl

#
# Issue #2: $0's crash on staticperl binaries
#
# This is a test where changing a value of $0 
# will crash Perl's binary on linux, solaris, ...
#
# Successfully survivied: freebsd, openbsd
# 

use strict;
use warnings;
use Proc::Pathname;

# let's see...
$0 = 'test';

printf "\$0: %s\n\$^X: %s\nprocname: %s\n",
        $0, $^X, Proc::Pathname::get() || "<undefined>";
