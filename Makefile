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

CFLAGS += -g -std=c99 $(WARNINGS) -ll -ly
 

.PHONY: all run flex bison compile clean clean-aux dist-clean

all: $(TARGETS) clean-aux

run: all
	./twitter-cli

lex.yy.c: twitter.l
	$(LEX) $^

y.tab.c: twitter.y
	$(YACC) $(BISONFLAGS) $^

twitter-cli: y.tab.c lex.yy.c
	$(CC) $(CFLAGS) -o $@ $^

flex: $(FLEX_INPUT)
	flex $(FLEX_INPUT)

bison: $(BISON_INPUT)
	bison $(BISON_INPUT) -yd 

compile: $(FLEX_OUTPUT) $(BISON_OUTPUT_C) $(BISON_OUTPUT_H)
	gcc -o $(OUTPUT) $(FLEX_OUTPUT) $(BISON_OUTPUT_C) -ll -ly

clean:
	rm -f $(TARGETS) $(OBJS)

clean-aux:
	rm -f $(OBJS)

dist-clean: clean

ex:
	./$(OUTPUT)
