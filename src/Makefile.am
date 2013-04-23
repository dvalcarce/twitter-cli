
BUILT_SOURCES = twitter_parser.h twitter_parser.c

bin_PROGRAMS = twitter-cli test-json

common_SOURCES = twitter_parser.y twitter_lexer.l json_utils.c

test_json_SOURCES = test-json.c $(common_SOURCES)
twitter_cli_SOURCES = twitter-cli.c http_lib.c $(common_SOURCES)


ALL_WARNINGS = -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
	    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
	    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
	    -Wuninitialized -Wconversion -Wstrict-prototypes

WARNINGS = -Wall

AM_CFLAGS = -g -std=c99 $(WARNINGS) -D_GNU_SOURCE
AM_YFLAGS = -y -d
