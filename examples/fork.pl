#!/usr/bin/perl

# unfinished yet a test with fork() call

use strict;
use warnings;
use Config;
use Proc::Pathname;

my $pid = fork ();

if (!defined $pid) {
        die "fork: $!";
}

if ( $pid != 0 ) { # parent
        waitpid $pid, 0;
        print "-" x 24, "\n";
        print "parent: $$\n";
        printf "\$0: %s\n\$^X: %s\nprocname: %s\n",
                $0, $^X, Proc::Pathname::get();
        printf "\$Config{_exe}: %s\n\$Config{perlpath}: %s\n",
                $Config{_exe}, $Config{perlpath};
} else { # child
        print "-" x 24, "\n";
        print "child: $$\n";
        printf "\$0: %s\n\$^X: %s\nprocname: %s\n",
                $0, $^X, Proc::Pathname::get();
        printf "\$Config{_exe}: %s\n\$Config{perlpath}: %s\n",
                $Config{_exe}, $Config{perlpath};
}
