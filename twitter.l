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

