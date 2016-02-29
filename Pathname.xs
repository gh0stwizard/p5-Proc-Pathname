/**
 * Copyright (C) 2016 by Vitaliy V. Tokarev
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the same terms as Perl itself.
 */

#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include "execname.h"


#if defined(_WIN32) || defined(_WIN64)
/* use free() provided by OS and NOT Perl's internal version */
static void
xfree(void *item)
{
#   undef free
    if (item != NULL)
        free (item);
}
#endif

MODULE = Proc::Pathname        PACKAGE = Proc::Pathname

PROTOTYPES: ENABLE

static SV *
get()
    CODE:
    char *path;
    int size;

    if ((size = get_proc_pathname (&path)) != -1) {
        RETVAL = newSVpvn_utf8(path, size, 1);
        /* 
         * http://www.perlmonks.org/?node_id=742205 
         */ 
#       if defined(_WIN32) || defined(_WIN64)
        xfree (path);
#       else
        free (path);
#       endif
    }
    else {
        RETVAL = &PL_sv_undef;
    }

    OUTPUT:
    RETVAL
