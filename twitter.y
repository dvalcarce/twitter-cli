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
#include "json_utils.h"

#define yyerror(...) fprintf(stderr, __VA_ARGS__)
// #define YYSTYPE struct json_value;

%}

%union {
	json_t j;
	struct json_object *obj;
	struct json_array *array;
}

// %error-verbose

%token <j> STRING
%token <j> NUMBER
%token <j> TRUE
%token <j> FALSE
%token <j> NIL
%token <j> ':'
%token <j> '['
%token <j> ']'
%token <j> '{'
%token <j> '}'
%token <j> ','

%type <j> S
%type <j> value
%type <j> object
%type <obj> members
%type <obj> pair
%type <j> array
%type <array> elements
%type <array> array_value

%start S

%%

S: value
	{
		printf("Valor final: "); pretty_print($1);
	}

object: '{' '}'						{ $$ = create_object(NULL); }
	| '{' members '}'				{ $$ = create_object($2); }

members: pair
	| pair ',' members				{ $$ = append_to_object($1, $3); }
	| error							{ yyerror("Malformed object."); YYABORT; }

pair: STRING ':' value				{ $$ = create_pair($1, $3); }

array: '[' ']'						{ $$ = create_array(NULL); }
	| '[' elements ']'				{ $$ = create_array($2); }

elements: array_value
	| array_value ',' elements		{ $$ = append_to_array($1, $3); }
	| error							{ yyerror("Malformed array."); YYABORT; }

array_value: value					{ $$ = create_array_value($1); }

value: STRING
	| NUMBER
	| object
	| array
	| TRUE
	| FALSE
	| NIL

%%
