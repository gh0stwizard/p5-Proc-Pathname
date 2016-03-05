package Proc::Pathname;


our $VERSION = '0.08';

require XSLoader;
XSLoader::load('Proc::Pathname', $VERSION);

1;
__END__

=pod

=encoding utf-8

=head1 NAME

Proc::Pathname - Retrieves the fully qualified path for the program

=head1 SYNOPSIS

  use Proc::Pathname;
  
  my $path = Proc::Pathname::get() or die "failed to get pathname";

=head1 DESCRIPTION

A common way to retrieve a path for the program is using the variable $0.
However, sometimes it is not working for you. So, you have to find a way 
to retrieve the path for the program somehow differently.

The module is using system calls to do the job and written as XS module.
This module was made for mine private purposes, mostly, for staticperl.


=head2 IMPLEMENTATION DETAILS

=over 4

=item * On GNU/Linux uses readlink /proc/self/exe

=item * On Windows uses GetModuleFileName() function

=item * On FreeBSD uses sysctl() system call

=item * On Solaris uses readlink /proc/self/path/a.out

=item * On Mac OS (Darwin) uses _NSGetExecutablePath() call (not tested)

=back 

=head2 EXPORT

None by default.

=head2 FUNCTIONS

There is only one function B<get()> and it is not exported.

=over 4

=item $path = B<get()>

Returns the fully qualified path for the program. If something goes 
wrong returns nothing (undef).

    my $path = Proc::Pathname::get()
        or die "failed to get proc pathname";

=back

=head1 AUTHOR

Vitaliy V. Tokarev, E<lt>vitaliy.tokarev@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2016 by Vitaliy V. Tokarev

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself.

=cut
