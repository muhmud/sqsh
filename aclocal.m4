dnl #   AC_VOLATILE (Scott C. Gray gray@voicenet.com)
dnl #
dnl #   Determines whether or not a compiler supports the volatile
dnl #   keyword.  Defines HAVE_VOLATILE
dnl #
AC_DEFUN(AC_VOLATILE,
[
	AC_MSG_CHECKING(whether ${CC-cc} supports volatile keyword)
	AC_CACHE_VAL(ac_cv_have_volatile,
	[
		AC_TRY_COMPILE(
			[#include <stdio.h>],
			[volatile int a ; a = 10 ;],
			ac_cv_have_volatile=yes, ac_cv_have_volatile=no )
	])
	AC_MSG_RESULT($ac_cv_have_volatile)

	if (test "$ac_cv_have_volatile" = "yes"); then
		AC_DEFINE(HAVE_VOLATILE, [1], [Define to 1 if your compiler supports the volatile keyword])
	fi
])

AC_DEFUN(AC_SIGSETJMP,
[
	AC_MSG_CHECKING(for sigsetjmp)
	AC_CACHE_VAL(ac_cv_have_sigsetjmp,
	[
		AC_TRY_COMPILE([
#include <stdio.h>
#include <setjmp.h>
		],[
			sigjmp_buf  jb;

			sigsetjmp(jb, 1);
		],[
			ac_cv_have_sigsetjmp=yes
		],[
			ac_cv_have_sigsetjmp=no
		])
	])

	AC_MSG_RESULT($ac_cv_have_sigsetjmp)

	if (test "$ac_cv_have_sigsetjmp" = "yes"); then
		AC_DEFINE(HAVE_SIGSETJMP, [1], [Define to 1 if your environment provides sigsetjpm()])
	fi
])

AC_DEFUN(AC_POSIX_SIGNALS,
[
	AC_MSG_CHECKING(for POSIX signal handling)
	AC_CACHE_VAL(ac_cv_have_posix_sigs,
	[
		AC_TRY_LINK(
		[
#include <stdio.h>
#include <signal.h>
   	],[
      		struct sigaction sa;
      		sigaction( SIGINT, NULL, &sa );
   	],[
				ac_cv_have_posix_sigs="yes"
		],[
				ac_cv_have_posix_sigs="no"
		])
   ])

	if test "$ac_cv_have_posix_sigs" = "yes"; then
		AC_DEFINE(HAVE_POSIX_SIGNALS, [1], [Define to 1 if your environment supprts POSIX signals])
	fi
	AC_MSG_RESULT($ac_cv_have_posix_sigs)
])

#
# AC_FIND_HEADER([hdr],[path],[action_found],[action_notfound])
#
AC_DEFUN([AC_FIND_HDR], [
	AC_MSG_CHECKING(for $1)
	ac_cache_var=ac_cv_hdr_`echo $1 | sed -e 's,[[/.]],_,g'`

	AC_CACHE_VAL($ac_cache_var,
		[
			SEARCH_PATH="/usr/include:/usr/local/include:${HOME}/include:${HOME}/usr/include"
			include_dir=""

			if test "$2" != ""; then
				SEARCH_PATH="$2:${SEARCH_PATH}"
			fi

			IFS="${IFS=  }"; ac_save_ifs="$IFS"; IFS="${IFS}:"
			for dir in ${SEARCH_PATH}; do
				if test -f ${dir}/$1; then
					include_dir=${dir}
					break;
				fi
			done
			IFS="$ac_save_ifs"

			eval ${ac_cache_var}=${include_dir}
		])

	include_dir=${ac_cache_var}
	include_dir=`eval echo '$'${include_dir}`

	if test "${include_dir}" != ""; then
		AC_MSG_RESULT($include_dir)
		$3
	else
		AC_MSG_RESULT(not found)
		$4
	fi
])

#
# AC_FIND_LIB([lib_name],[path],[action_found],[action_notfound])
#
AC_DEFUN(AC_FIND_LIB, [
	AC_MSG_CHECKING(for lib$1)
	AC_CACHE_VAL(ac_cv_lib$1,
		[
			SEARCH_PATH="/lib:/usr/lib:/usr/local/lib:${HOME}/lib:${HOME}/usr/lib"
			libdir=""

			if test "${LD_LIBRARY_PATH}" != ""; then
				SEARCH_PATH="${LD_LIBRARY_PATH}:${SEARCH_PATH}"
			fi

			if test "$2" != ""; then
				SEARCH_PATH="$2:${SEARCH_PATH}"
			fi

			IFS="${IFS=  }"; ac_save_ifs="$IFS"; IFS="${IFS}:"
			for dir in ${SEARCH_PATH}; do
				for lib in ${dir}/lib$1.*; do
					if test -f "${lib}"; then
						libdir=${dir}
						break;
					fi
				done

				if test "${libdir}" != ""; then
					break;
				fi
			done
			IFS="$ac_save_ifs"

			ac_cv_lib$1="${libdir}"
		])

	libdir=$ac_cv_lib$1

	if test "${libdir}" != ""; then
		AC_MSG_RESULT($libdir)
		$3
	else
		AC_MSG_RESULT(not found)
		$4
	fi
])


dnl #   AC_SYSV_SIGNALS  (Scott C. Gray gray@voicenet.com)
dnl #
dnl #   Determines behaviour of signals, if they need to be reinstalled
dnl #   immediately after calling.
dnl #
AC_DEFUN(AC_SYSV_SIGNALS,
[
	AC_MSG_CHECKING(signal behaviour)
	AC_CACHE_VAL(ac_cv_signal_behaviour,
	[
		AC_TRY_RUN(
		[
/*-- Exit 0 if BSD signal, 1 if SYSV signal --*/
#include <signal.h>
static int sg_sig_count = 0 ;
void sig_handler() { ++sg_sig_count ; }
main() {
#ifdef SIGCHLD
#define _SIG_ SIGCHLD
#else
#define _SIG_ SIGCLD
#endif
    signal( _SIG_, sig_handler ) ;
    kill( getpid(), _SIG_ ) ;
    kill( getpid(), _SIG_ ) ;
    exit( sg_sig_count != 2 ) ;
}
		], ac_cv_signal_behaviour=BSD, ac_cv_signal_behaviour=SYSV)
	])

	AC_MSG_RESULT($ac_cv_signal_behaviour)

	if (test "$ac_cv_signal_behaviour" = "SYSV"); then
		AC_DEFINE(SYSV_SIGNALS, [1], [Define to 1 if your environment supports SysV signals])
	fi
])



AC_DEFUN([AC_SYBASE_ASE], [

	#
	#  Is $SYBASE defined?
	#
	AC_MSG_CHECKING(Open Client installation)
		if test "$SYBASE" = ""; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([Unable to locate Sybase installation. Check your SYBASE environment variable setting.])
		fi

		SYBASE_OCOS=
		for subdir in $SYBASE $SYBASE/OCS $SYBASE/OCS-[[0-9]]*
		do
			if test -d $subdir/include; then
				SYBASE_OCOS=$subdir
				break
			fi
		done

		if test "$SYBASE_OCOS" = ""; then
			AC_MSG_RESULT(fail)
			AC_MSG_ERROR([Sybase include files not found in \$SYBASE. Check your SYBASE environment variable setting.])
		fi
	AC_MSG_RESULT($SYBASE_OCOS)

	AC_MSG_CHECKING(Open Client libraries)

		if test "$with_devlib" = "yes"
		then
			SYBASE_LIBDIR="$SYBASE_OCOS/devlib"
		else
			SYBASE_LIBDIR="$SYBASE_OCOS/lib"
		fi

		SYBASE_LIBS=

		for i in blk cs ct tcl comn intl unic
		do
			x=
			if test -f $SYBASE_LIBDIR/lib${i}.a; then
				x="-l${i}"
			else if test -f $SYBASE_LIBDIR/lib${i}64.a; then
				x="-l${i}64"
			else if test -f $SYBASE_LIBDIR/libsyb${i}.a; then
				x="-lsyb${i}"
			else if test -f $SYBASE_LIBDIR/libsyb${i}64.a; then
				x="-lsyb${i}64"
			fi
			fi
			fi
			fi
			if test -n $x ; then
				SYBASE_LIBS="$SYBASE_LIBS $x"
			fi
		done
		#
		# Check for -ltds (FreeTDS project)
		#
		if test -f $SYBASE_LIBDIR/libtds.a; then
			SYBASE_LIBS="$SYBASE_LIBS -ltds"
		fi

	AC_MSG_RESULT($SYBASE_LIBS)

	AC_MSG_CHECKING(Open Client needs net libraries)

		SYBASE_VERSION=`strings $SYBASE_LIBDIR/lib*ct.a 2>/dev/null | \
			cut -c1-80 | fgrep 'Sybase Client-Library' | cut -d '/' -f 2`
		SYBASE_VERSION=`echo $SYBASE_VERSION | cut -d . -f 1`

		if [[ "$SYBASE_VERSION" = "" ]]; then
			SYBASE_VERSION="FreeTDS"
		fi

		if [[ "$SYBASE_VERSION" = "10" ]]; then
			if [[ -f $SYBASE_LIBDIR/libinsck.a ]]; then
				SYBASE_LIBS="$SYBASE_LIBS -linsck"
				AC_MSG_RESULT([yes [(]-linsck[)]])
			elif [[ -f $SYBASE_LIBDIR/libtli.a ]]; then
				SYBASE_LIBS="$SYBASE_LIBS -ltli"
				AC_MSG_RESULT([yes [(]-ltli[)]])
			else
				AC_MSG_RESULT([yes [(]version 10.x[)]])
			fi
		else
			AC_MSG_RESULT([no [(]version $SYBASE_VERSION[)]])
		fi
		

	AC_MSG_CHECKING(Open Client OS libraries)
		case "${host_os}" in
			linux*)
				SYBASE_OS="-ldl -lm";;
			irix*)
				SYBASE_OS="-lnsl -lm";;
			ncr*)
				SYBASE_OS="-ldl -lm";;
			sunos*)
				SYBASE_OS="-lm";;
			solaris*)
				SYBASE_OS="-lnsl -ldl -lm";;
			osf1*)
				SYBASE_OS="-lsdna -ldnet_stub -lm";;
			ultrix*)
				SYBASE_OS="-lsdna -ldnet_stub -lm";;
			hpux*)
				SYBASE_OS="-lcl -lm -lsec -lBSD";;
			dgux*)
				SYBASE_OS="-lm -ldl -ldgc";;
			aix*)
				SYBASE_OS="-lm";;
			*)
				SYBASE_OS="-lm -ldl";;
		esac

	AC_MSG_RESULT($SYBASE_OS)

	if test "$with_static" = "yes"
	then
		SYBASE_LIBS=`echo $SYBASE_LIBS | \
			sed -e 's,-l\([[a-z]]*\),\$(SYBASE_OCOS)/lib/lib\1.a,g'`
	fi

	if test "$with_devlib" = "yes"; then
		SYBASE_LIBDIR='-L$(SYBASE_OCOS)/devlib'
	else
		SYBASE_LIBDIR='-L$(SYBASE_OCOS)/lib'
	fi
	SYBASE_INCDIR='-I$(SYBASE_OCOS)/include'

	AC_SUBST(SYBASE_OCOS)
	AC_SUBST(SYBASE_LIBS)
	AC_SUBST(SYBASE_LIBDIR)
	AC_SUBST(SYBASE_INCDIR)
	AC_SUBST(SYBASE_OS)
	AC_SUBST(SYBASE)
])

