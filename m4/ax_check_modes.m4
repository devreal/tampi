#
# SYNOPSIS
#
#   AX_CHECK_BLOCKING_MODE
#
#   AX_CHECK_NONBLOCKING_MODE
#

AC_DEFUN([AX_CHECK_BLOCKING_MODE], [
	AC_MSG_CHECKING([whether the blocking mode of TAMPI is enabled])
	AC_ARG_ENABLE(
		[blocking-mode],
		[AS_HELP_STRING([--disable-blocking-mode], [do not compile the blocking mode of TAMPI [default=enabled]])],
		[
			case "${enableval}" in
			yes)
				AC_MSG_RESULT([yes])
				ac_blocking_mode=yes
				;;
			no)
				AC_MSG_RESULT([no])
				ac_blocking_mode=no
				;;
			*)
				AC_MSG_ERROR([bad value ${enableval} for --disable-blocking-mode])
				;;
			esac
		],
		[
			AC_MSG_RESULT([yes])
			ac_blocking_mode=yes
		]
	)
	if test x"${ac_blocking_mode}" = x"no" ; then
		AC_DEFINE([HAVE_BLOCKING_MODE], 0, [Compile the blocking mode of TAMPI])
	else
		AC_DEFINE([HAVE_BLOCKING_MODE], 1, [Compile the blocking mode of TAMPI])
	fi
])

AC_DEFUN([AX_CHECK_NONBLOCKING_MODE], [
	AC_MSG_CHECKING([whether the non-blocking mode of TAMPI is enabled])
	AC_ARG_ENABLE(
		[nonblocking-mode],
		[AS_HELP_STRING([--disable-nonblocking-mode], [do not compile the non-blocking mode of TAMPI [default=enabled]])],
		[
			case "${enableval}" in
			yes)
				AC_MSG_RESULT([yes])
				ac_nonblocking_mode=yes
				;;
			no)
				AC_MSG_RESULT([no])
				ac_nonblocking_mode=no
				;;
			*)
				AC_MSG_ERROR([bad value ${enableval} for --disable-nonblocking-mode])
				;;
			esac
		],
		[
			AC_MSG_RESULT([yes])
			ac_nonblocking_mode=yes
		]
	)
	if test x"${ac_nonblocking_mode}" = x"no" ; then
		AC_DEFINE([HAVE_NONBLOCKING_MODE], 0, [Compile the non-blocking mode of TAMPI])
	else
		AC_DEFINE([HAVE_NONBLOCKING_MODE], 1, [Compile the non-blocking mode of TAMPI])
	fi
])
