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
	#include "y.tab.h"

	void cp_num()
	{
		yylval.num = atoll(yytext);
	}

	void cp_str(int remove_quotes)
	{
		if (remove_quotes) {
			yylval.str = strndup(yytext + 1, yyleng - 2);
		} else {
			yylval.str = strdup(yytext);
		}
	}

	void cp_real()
	{
		yylval.real = atof(yytext);
	}

%}


%%

<INITIAL>.*				{ cp_str(0); return SOMETHING; }

%%

