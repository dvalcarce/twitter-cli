#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([twitter-cli], [0.1], [https://github.com/dvalcarce/twitter-cli/issues], [twitter-cli], [https://github.com/dvalcarce/twitter-cli])

AC_REVISION([$Id$])

AC_CONFIG_SRCDIR([src/headers.h])
AC_CONFIG_HEADERS([src/config.h])

AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_YACC
AC_PROG_CC
AC_PROG_LEX
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostbyname memmove memset socket strdup strncasecmp strpbrk strtol])


AC_CONFIG_FILES([Makefile
                 src/Makefile
])
#                 contrib/Makefile])

# AC_CONFIG_SUBDIRS([contrib/liboauth-1.0.1])


AC_OUTPUT
