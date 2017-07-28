/* src/config.h.  Generated from config.h.in by configure.  */
#ifndef config_h_included
#define config_h_included

/*
 * Define the following if you compiler is an ANSI compiler and supports
 * prototypes, etl a.
 */
#define STDC_HEADERS 1

/*
 * The include file sys/wait.h is required by sqsh_sigcld.c in order
 * to call wait_pid().  Define this if the header file is available.
 */
#define HAVE_SYS_WAIT_H 1

/*
 * Common header files.
 */
#define HAVE_STDLIB_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STRING_H 1
#define HAVE_FCNTL_H 1
#define HAVE_UNISTD_H 1
#define HAVE_MEMORY_H 1
#define HAVE_ERRNO_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_TIME_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_LIMITS_H 1
#define HAVE_CRYPT_H 1
/* #undef HAVE_SHADOW_H */
/* #undef HAVE_STROPTS_H */

/*
 * If you compiler doesn't fully support the keyword 'const'
 * then define const to be empty.
 */
/* #undef const */

/*
 * If the following types are not defined in stdlib.h and sys/types.h
 * then guess the closest possible data type.
 */
/* #undef mode_t */
/* #undef pid_t */
/* #undef uid_t */

/*
 * The following is the return type of a signal handler.  On most
 * systems this is void.
 */
#define RETSIGTYPE void

/*
 * The following functions are frequently unavailable on certain
 * systems.
 */
#define HAVE_STRCASECMP 1
#define HAVE_STRERROR 1
/* #undef HAVE_CFTIME */
#define HAVE_STRFTIME 1
/* #undef HAVE_MEMCPY */
/* #undef HAVE_MEMMOVE */
#define HAVE_LOCALTIME 1
#define HAVE_TIMELOCAL 1
#define HAVE_STRCHR 1
#define HAVE_SIGSETJMP 1
#define HAVE_GETTIMEOFDAY 1
/* #undef HAVE_GET_PROCESS_STATS */
#define HAVE_SIGACTION 1
/* #undef HAVE_CRYPT */
#define HAVE_POLL 1

/*
 * Define if your compiler supports the volatile keyword.
 */
#define HAVE_VOLATILE 1

/*
 * Signal behaviour
 */
/* #undef SYSV_SIGNALS */

/*
 * sqsh-2.3 : Test availability of locale
 */
#define HAVE_LOCALE_H 1
#define HAVE_LOCALECONV 1
#define HAVE_SETLOCALE 1

#endif
