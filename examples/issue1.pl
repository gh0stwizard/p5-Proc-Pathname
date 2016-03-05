#!/usr/bin/perl

# Note: this is a test file for staticperl binary
# Issue: 
#  * `(deleted)' at the end (on gnu/linux at least)
#  * on openbsd is undef (as version 0.06)

use strict;
use warnings;
use Proc::Pathname;

unlink 'issue1';
printf "\$0: %s\n\$^X: %s\nprocname: %s\n",
        $0, $^X, Proc::Pathname::get() || "<undefined>";
