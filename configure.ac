#                                               -*- Autoconf -*-

AC_PREREQ(2.61)
AC_INIT([dta1d], [0.0.1], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])



AC_PROG_CC
AC_CONFIG_SRCDIR([src/project.c])
AC_CONFIG_HEADERS([config.h])


# Checks for libraries.
AM_PATH_GLIB_2_0(2.6.0,,AC_MSG_ERROR(GLIB+ >= 2.6.0 not installed.))
AM_PATH_GTK_3_0(3.2.0,,AC_MSG_ERROR(GTK+ >= 3.2.0 not installed.))

AC_CONFIG_FILES([
      Makefile
      src/Makefile
])
AC_OUTPUT
