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

%}

%union{
	struct json_value;
}

%error-verbose

%token <str> STRING
%token NUMBER
%token TRUE
%token FALSE
%token NIL
%token ':'
%token '['
%token ']'
%token '{'
%token '}'
%token ','

%start S

%%

S: value
	{
		printf("%s\n", $1);	
	}

object: '{' members '}'

members: pair
       | pair ',' members

pair: STRING ':' value

array: '[' ']'
     | '[' members ']'

elements: value
	| value ',' elements

value: STRING
     | NUMBER
     | object
     | array
     | TRUE
     | FALSE
     | NULL
     

%%

int
main(int argc, char *argv[])
{
	return 0;
}
