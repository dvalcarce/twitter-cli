BISON_INPUT=twitter.y
FLEX_INPUT=twitter.l
BISON_OUTPUT_C=y.tab.c
BISON_OUTPUT_H=y.tab.h
FLEX_OUTPUT=lex.yy.c
OUTPUT=cli

BISONFLAGS = -yd

SOURCES = twitter.y twitter.l
OBJS = y.tab.c y.tab.h lex.yy.c
TARGETS = twitter-cli

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
	    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
	    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
	    -Wuninitialized -Wconversion -Wstrict-prototypes

WARNINGS := -Wall

CFLAGS += -g -std=c99 $(WARNINGS)
LDFLAGS += -lfl -ly


.PHONY: all run flex bison compile clean clean-aux dist-clean

all: $(TARGETS) clean-aux

run: all
	./twitter-cli

lex.yy.c: twitter.l
	$(LEX) $^

y.tab.h: y.tab.c

y.tab.c: twitter.y
	$(YACC) $(BISONFLAGS) $^

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c $@ $^

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c $@ $<


twitter-cli: twitter-cli.o lex.yy.o y.tab.o json_utils.o
	$(LD) -lc $(LDFLAGS) -o $@ $^

test-json: test-json.o lex.yy.o y.tab.o json_utils.o
	gcc -o $@ $^ -lc $(LDFLAGS)
	# $(LD) -lc $(LDFLAGS) -o $@ -L/lib/crt0.o $^




flex: $(FLEX_INPUT)
	flex $(FLEX_INPUT)

bison: $(BISON_INPUT)
	bison $(BISON_INPUT) -yd

compile: $(BISON_OUTPUT_C) $(FLEX_OUTPUT) $(BISON_OUTPUT_H) json_utils.o
	gcc -o $(OUTPUT) $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS) $(OBJS)

clean-aux:
	rm -f $(OBJS)

dist-clean: clean

ex:
	./$(OUTPUT)
