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
/**
 * Utilities file for dealing with JSON.
 */

#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "headers.h"

typedef struct _json_iter_context {
	union {
		struct json_object *obj;
		struct json_array *arr;
	};
	void *ref;
} json_context_t;

void pretty_print(json_t);
json_t empty_array(void);
json_t create_object(struct json_object *);
json_t create_array(struct json_array *);
struct json_object *create_pair(json_t, json_t);
struct json_object *append_to_object(struct json_object*, struct json_object*);

struct json_array *create_array_value(json_t);
struct json_array *append_to_array(struct json_array *, struct json_array *);
void json_free(json_t *);


/* Getters / setters */
enum json_type json_get_type(json_t *val);
json_t *json_object_get_value(json_t *val, stringl_t key);
json_t *json_array_index(json_t *val, int pos);
long json_array_len(json_t *val);
char *json_as_string(json_t *val);
int json_as_bool(json_t *val);
int json_is_null(json_t *val);
long long json_as_int(json_t *val);

/* Iterators */
json_context_t json_context_init();
json_t *for_array_r(json_t *val, json_context_t *ctx);
json_t *for_object_r(json_t *val, json_context_t *ctx, char **key);


#endif
