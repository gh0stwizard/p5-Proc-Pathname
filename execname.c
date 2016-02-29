/**
 * Copyright (C) 2016 by Vitaliy V. Tokarev
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the same terms as Perl itself.
 */

#include <stdio.h>      /* fprintf() */
#include <stdlib.h>     /* malloc(), free() */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


#if defined(__linux__)
    /* Linux. --------------------------------------------------- */
#if defined(_GNU_SOURCE) && defined(GNU_PROGNAME)
#include <errno.h>
#include <string.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#endif
#elif defined(_WIN32) || defined(_WIN64)
    /* Microsoft Windows ---------------------------------------- */
#include <windows.h>
#include <errno.h>
#elif defined(__sun) && defined(__SVR4)
    /* Solaris. ------------------------------------------------- */
#include <unistd.h>
#include <string.h>
#include <limits.h>
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    /* Mac OS, Darwin ------------------------------------------- */
#include <sys/param.h>
#warning not tested
#if defined(BSD)
    /* BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD). ----------- */
#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#include <errno.h>
#include <string.h>
#endif
#endif
#else
#error unknown operating system
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * error: print an error message to stderr
 */
static void
error (const char *msg, int line)
{
    fprintf (stderr, "get_proc_pathname: %s at line %d\n", msg, line);
}


/**
 * get_proc_pathname: returns length of the process pathname and stores
 * an actual pathname into *outpath. Otherwise, returns -1 on error.
 */
int
get_proc_pathname (char **outpath)
{
#if defined(__linux__)
    /* Linux. --------------------------------------------------- */
    int len;
#if defined(_GNU_SOURCE) && defined(GNU_PROGNAME)
    /* meanwhile this is simplest implementation the value returned
     * by this way is NOT an absolute path to a program
     */
    extern char *program_invocation_name;
    len = strlen (program_invocation_name) + 1; /* plus null-byte */
    *outpath = program_invocation_name;
#else
    static ssize_t rlsize;
    static char linkname[PATH_MAX];

    rlsize = readlink ("/proc/self/exe", linkname, PATH_MAX - 1);
    if (rlsize == -1) {
        error (strerror (errno), __LINE__);
        goto error;
    }

    if (rlsize > PATH_MAX - 1) {
        error ("symlink name's length greater than PATH_MAX", __LINE__);
        goto error;
    }

    linkname[rlsize] = '\0';

    *outpath = (char *)malloc (rlsize * sizeof (char));
    strncpy (*outpath, linkname, rlsize);
    goto done;

error:
    len = -1;

done:
    len = rlsize;

#endif
#elif defined(_WIN32) || defined(_WIN64)
    /* Microsoft Windows ---------------------------------------- */
    wchar_t buffer[MAX_PATH]; 	/* temporary buffer for path */
    wchar_t *path = NULL; 		/* actual path in unicode */
    wchar_t *abspath = NULL; 	/* absolute path in unicode */
    char *out = NULL;           /* an absolute path (mb) */
    DWORD err = NO_ERROR;
    DWORD len = 0;

    len = GetModuleFileNameW (NULL, buffer, ARRAY_SIZE(buffer));

    switch (err = GetLastError ()) {
    case NO_ERROR:
        path = buffer;
        break;
    case ERROR_INSUFFICIENT_BUFFER:
        do {
            len *= 2;
            if ((path = (wchar_t *)realloc (path, len * sizeof (path))) == NULL) {
                error ("realloc error", __LINE__);
                goto error;
            }
            len = GetModuleFileNameW (NULL, path, len);
            err = GetLastError ();
        } while (err == ERROR_INSUFFICIENT_BUFFER);
        /* passthrough */
    default:
        if (err != NO_ERROR) {
            error (strerror (err), __LINE__);
            goto error;
        }
        break;
    }

    /* resolve an absolute path */
    if ((abspath = _wfullpath (NULL, path, sizeof (path))) == NULL) {
        error (strerror (GetLastError ()), __LINE__);
        goto error;
    }

    /* calculate the length for  char *out */
    len = WideCharToMultiByte (CP_UTF8, 0, abspath, -1, NULL, 0, NULL, NULL);

    if ((err = GetLastError ()) == NO_ERROR) {
        /* allocate space for output value (mb) */
        if ((out = (char *)realloc (out, len * sizeof (char))) == NULL) {
            error ("realloc error", __LINE__);
            goto error;
        }
        /* convert a widechar value of "abspath" to a multibyte "out" */
        if ((len = WideCharToMultiByte (CP_UTF8, 0, abspath, -1, 
                        out, len, NULL, NULL)) > 0)
        {
            *outpath = out;
            goto done;
        }
        else {
            error (strerror (GetLastError ()), __LINE__);
        }
    }
    else {
        error (strerror (err), __LINE__);
    }

error:
    len = -1;

    if (out != NULL)
        free (out);

done:
    if (path != NULL && path != buffer)
        free (path);

    if (abspath != NULL)
        free (abspath);

#elif defined(__sun) && defined(__SVR4)
    int len;
#if defined (SUN_GETEXECNAME)
    /**
     * very simple and useless, because it's returns relative path
     */
    *outpath = getexecname ();
    len = strlen (*outpath);
#else
/*#if defined (__SUNPRO_C)*/
    /* Solaris: Sun Compiler. ----------------------------------- */
/*#elif defined (__GNUC__) && __GNUC__ >= 2*/
    /* Solaris: GCC/Clang Compiler. ----------------------------- */
    static char tmp[PATH_MAX];
    static ssize_t rlsize;

    rlsize = readlink ("/proc/self/path/a.out", tmp, PATH_MAX - 1);
    if (rlsize == -1) {
        error ("readlink", __LINE__);
        goto error;
    }

    tmp[rlsize] = '\0';
    if ((*outpath = (char *)malloc (rlsize * sizeof (char))) != NULL) {
        strncpy (*outpath, tmp, rlsize);
        goto done;
    }

error:
    len = -1;

done:
    len = rlsize;

#endif
/*#endif*/
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    /* Mac OS, Darwin ------------------------------------------- */
    char path[PATH_MAX];
    uint32_t size = sizeof (path);

    if (_NSGetExecutablePath (path, &size) == 0) {
        if ((*outpath = (char *)malloc (cb * sizeof (char))) != NULL) {
            strncpy (*outpath, tmp, cb);
            len = size;
        }
        else {
            error ("malloc error", __LINE__);
            len = -1;
        }
    }
    else {
        error ("_NSGetExecutablePath error", __LINE__);
        len = -1;
    }

#elif defined(BSD)
#if defined(__FreeBSD__)
    /* BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD). ----------- */
    int len;
    /**
     * code taken from:
     * http://stackoverflow.com/questions/799679/programatically-retrieving-the-absolute-path-of-an-os-x-command-line-app
     */
    static int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    static char tmp[PATH_MAX];
    size_t cb = sizeof (tmp);

    if (sysctl (mib, 4, tmp, &cb, NULL, 0) == -1) {
        error (strerror (errno), __LINE__);
        len = -1;
    }
    else if ((*outpath = (char *)malloc (cb * sizeof (char))) != NULL) {
        strncpy (*outpath, tmp, cb);
        len = cb;
    }
    else {
        error ("malloc error", __LINE__);
        len = -1;
    }
#endif
#else
#error unknown operating system
#endif

    /* return the length of pathname */	
    return len;
}

#ifdef __cplusplus
}
#endif
