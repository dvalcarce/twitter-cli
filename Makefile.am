
SUBDIRS = src $(ac_subdirs)
EXTRA_DIST = autogen.sh LEEME.md src/headers.h src/twitter_keys.h src/*.h \
	     contrib/curl-7.30.0.tar.gz \
	     contrib/liboauth-1.0.1.tar.gz \
	     contrib/openssl-1.0.1e.tar.gz

AM_CPPFLAGS = -I@libcurl@ -I@liboauth@
AM_LDFLAGS = -Llibs

config:
	echo $(AM_CPPFLAGS)

