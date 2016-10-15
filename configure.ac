#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# m4_define([libpng_required_version], [1.2.22])

AC_PREREQ(2.61)
AC_INIT([dta1d], [0.0.1], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/project.cpp])
AC_CONFIG_HEADER([config.h])
#AC_OBJEXT(c)



# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CXX
AC_PROG_RANLIB
#AC_LANG(C)
##AM_PROG_LIBTOOL

AM_CONDITIONAL(HAVE_GCC, [test "$GCC" = "yes"])

# Checks for libraries.
AM_PATH_GLIB_2_0(2.6.0,,AC_MSG_ERROR(GLIB+ >= 2.6.0 not installed.))
AM_PATH_GTK_3_0(3.2.0,,AC_MSG_ERROR(GTK+ >= 3.2.0 not installed.))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([bzero])

AC_CONFIG_FILES([Makefile
		src/Makefile])
AC_OUTPUT
