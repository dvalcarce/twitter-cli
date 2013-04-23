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
#ifndef _HEADER_H_
#define _HEADER_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


#ifndef STRING_MAX
#define STRING_MAX 400
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#include <assert.h>
#else
#define assert(x) {}
#endif



enum json_type { JSON_OBJECT, JSON_ARRAY, JSON_STRING, JSON_INT, JSON_FLOAT, JSON_T, JSON_F, JSON_NIL };

typedef struct json_string {
	char *val;
	int len;
} string_t;

typedef char stringl_t[STRING_MAX];


union json_value_u {
	stringl_t s;
	long long i;
	double f;
	struct json_object *as_obj;
	struct json_array *as_array;
};

struct json_value {
	enum json_type type;
	union json_value_u value;
};
typedef struct json_value json_t;


struct json_object {
	stringl_t key;
	json_t value;
	struct json_object *next;
};

struct json_array {
	json_t value;
	struct json_array *next;
};

#endif
