BISON_INPUT=twitter.y
FLEX_INPUT=twitter.l
BISON_OUTPUT_C=y.tab.c
BISON_OUTPUT_H=y.tab.h
FLEX_OUTPUT=lex.yy.c
OUTPUT=cli

all: flex bison compile clean

flex: $(FLEX_INPUT)
	flex $(FLEX_INPUT)

bison: $(BISON_INPUT)
	bison $(BISON_INPUT) -yd 

compile: $(FLEX_OUTPUT) $(BISON_OUTPUT_C) $(BISON_OUTPUT_H)
	gcc -o $(OUTPUT) $(FLEX_OUTPUT) $(BISON_OUTPUT_C) -ll -ly

run:
	./$(OUTPUT)

clean:
	rm -f $(FLEX_OUTPUT) $(BISON_OUTPUT_C) $(BISON_OUTPUT_H)

ex:
	./$(OUTPUT)
