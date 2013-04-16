/*
 *	Santiago Saavedra López
 *	Daniel Valcarce Silva
 *
 *	Práctica final
 *	Compiladores
 *	2012/2013
 */

%{
	#include "headers.h"

%}

%union{
	long long num;
	char *str;
	double real;
	unsigned flag;
}

%error-verbose

%token <str> SOMETHING

%start S

%%

S: SOMETHING
	{
		printf("%s\n", $1);	
	}

%%

int
main(int argc, char *argv[])
{
	return 0;
}
