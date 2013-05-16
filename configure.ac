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


AC_ARG_ENABLE([libcurl], AS_HELP_STRING([--disable-libcurl], [Disable builtin libcurl]))
AC_ARG_ENABLE([liboauth], AS_HELP_STRING([--disable-liboauth], [Disable builtin liboauth]))
AC_ARG_ENABLE([openssl], AS_HELP_STRING([--disable-openssl], [Disable builtin OpenSSL for liboauth]))

ac_contrib_dir=`readlink -f $srcdir`/contrib
ac_liboauth_cflags=""
ac_liboauth_ldflags=""

AS_IF([test "x$enable_openssl" != "xno"], [
	       AC_MSG_CHECKING([openssl])
	AS_IF([test -f contrib/openssl-1.0.1e/libssl.a], [
		AC_MSG_RESULT([Already done])
	       ],
	       [
	       ( cd contrib && tar -xzf openssl-1.0.1e.tar.gz && cd openssl-1.0.1e && ./config --prefix="$ac_contrib_dir/openssl-1.0.1e" && make )
	       PWD=`pwd`
	       AC_MSG_RESULT([ok])
	       ])
	       ac_liboauth_cflags="-I$ac_contrib_dir/openssl-1.0.1e/include"
	       ac_liboauth_ldflags="-L$ac_contrib_dir/openssl-1.0.1e"
	],
       [
		AC_MSG_CHECKING([openssl])
		AC_CHECK_HEADER([openssl/lhash.h],, [AC_MSG_ERROR([OpenSSL not found.])])
	])
	       

# AC_ARG_ENABLE([unicode-characters], AS_HELP_STRING([--enable-unicode-characters], [Enable unicode characters in RT / Favs]))

AC_ARG_ENABLE([unicode],
	      AS_HELP_STRING([--enable-unicode], [Enable unicode characters in RT/Fav display (disabled by default)]),
	      [if test x$enableval = xyes; then
			AC_DEFINE([HAVE_UNICODE_ENABLED], [1], [Define this to enable unicode characters in RT and Favs])
		fi])



AS_IF([test "x$enable_libcurl" != "xno"], [
        AC_MSG_CHECKING([libcurl])
	AS_IF([test -f contrib/curl-7.30.0/lib/.libs/libcurl.so], [
	       AC_MSG_RESULT([Already done])
	       ],
	       [
		( cd contrib && tar -zxf curl-7.30.0.tar.gz && cd curl-7.30.0 && PKG_CONFIG_PATH="$ac_contrib_dir/openssl-1.0.1e" ./configure CPPFLAGS="$ac_liboauth_cflags" LDFLAGS="$ac_liboauth_ldflags" --with-ssl="$ac_contrib_dir/openssl-1.0.1e" && make)
		( cd libs && ln -s $srcdir/contrib/curl-7.30.0/lib/.libs/libcurl.so )]
	       AC_MSG_RESULT([done]))
       AC_SUBST([libcurl], [$srcdir/contrib/libcurl-7.30.0/include])
       ])
# 
AS_IF([test "x$enable_liboauth" != "xno"], [
        AC_MSG_CHECKING([liboauth])
	AS_IF([test -f contrib/liboauth-1.0.1/src/.libs/liboauth.so], [
	       AC_MSG_RESULT([Already done])
	       ],
	       [
		( cd contrib && tar -zxf liboauth-1.0.1.tar.gz && \
		 cd liboauth-1.0.1 && \
		 ./configure CFLAGS="$ac_liboauth_cflags" LDFLAGS="$ac_liboauth_ldflags" && make)
		( cd libs && ln -s $srcdir/contrib/liboauth-1.0.1/src/.libs/liboauth.so )
		AC_MSG_RESULT([done])])
 
#        ( cd contrib && tar -xzf liboauth-1.0.1.tar.gz )
#        AC_CONFIG_SUBDIRS([contrib/liboauth-1.0.1])
       AC_SUBST([liboauth], [$srcdir/contrib/liboauth-1.0.1/src])
       ])



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
