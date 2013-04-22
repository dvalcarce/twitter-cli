/***************************************************************************
 *   Twitter client for the CLI.                                           *
 *   Copyright (C) 2013 by Santiago Saavedra and Daniel Valcarce           *
 *   <santiagosaavedra@gmail.com>, <danivalsil@gmail.com>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
/***************************************************************************
 *   This is made as a final exercise for Compilers in the University of   *
 *   A Coruna.                                                             *
 *   Year: 2012/2013                                                       *
 **************************************************************************/

%{
	#include "headers.h"
	#include "y.tab.h"

	void cp_str()
	{
		yylval.j.type = JSON_STRING;
		strncpy(yylval.j.value.s, yytext + 1, MAX(yyleng - 2, STRING_MAX));
	}

	void cp_number()
	{
		if (strpbrk(yytext, ".eE") != NULL) {
			yylval.j.value.f = atof(yytext);
			yylval.j.type = JSON_FLOAT;
		} else {
			yylval.j.value.i = atoll(yytext);
			yylval.j.type = JSON_INT;
		}
	}

	void cp_simple(enum json_type type)
	{
		yylval.j.type = type;
	}

%}

%x xq

quote					[\"]
hex						[0-9A-Fa-f]
unicode					\\u{hex}{4}
specialchar				[\\][bfnrt/\\]|{unicode}
char					{specialchar}|[A-Za-z]
string					{quote}{char}*{quote}

e						[eE]([+-]?)
digit1					[1-9]
digit					[0-9]
exp						{e}{digit}+
frac					"."{digit}+
int						"-"?{digit}|({digit1}{digit}+)
number					{int}({frac}|{exp}|{frac}{exp})?

true					"true"
false					"false"
null					"null"
colon					":"
o_bracket				"["
c_bracket				"]"
o_brace					"{"
c_brace					"}"
comma					","

ws						[ \n\t]

%%

{string}				{ cp_str(); return STRING; }
{number}				{ cp_number(); return NUMBER; }

{true}					{ cp_simple(JSON_T); return TRUE; }
{false}					{ cp_simple(JSON_F); return FALSE; }
{null}					{ cp_simple(JSON_NIL); return NIL; }
{colon}					{ return ':'; }
{o_bracket}				{ return '{'; }
{c_bracket}				{ return '}'; }
{o_brace}				{ return '['; }
{c_brace}				{ return ']'; }
{comma}					{ return ','; }

{ws}					{ }

%%

