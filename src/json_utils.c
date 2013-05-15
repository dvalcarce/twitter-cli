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

#include <stdio.h>
#include "headers.h"
#include "json_utils.h"


void print_value3(json_t v, char *terminator, FILE *out)
{
	switch(v.type) {
		case JSON_STRING:
			fprintf(out, "\"%s\"%s", v.value.s, terminator);
			break;
		case JSON_INT:
			fprintf(out, "%lld%s", v.value.i, terminator);
			break;
		case JSON_FLOAT:
			fprintf(out, "%lf%s", v.value.f, terminator);
			break;
		case JSON_ARRAY:
			fprintf(out, "[Array]%s", terminator);
			break;
		case JSON_OBJECT:
			fprintf(out, "[Object]%s", terminator);
			break;
		case JSON_T:
			fprintf(out, "true%s", terminator);
			break;
		case JSON_F:
			fprintf(out, "false%s", terminator);
			break;
		case JSON_NIL:
			fprintf(out, "null%s", terminator);
			break;
	}
}

void print_value(json_t v)
{
	print_value3(v, "\n", stdout);
}

void pretty_print4(json_t v, int indentation, char *terminator, FILE *out)
{
	int i;
	struct json_object *o = v.value.as_obj;
	struct json_array *a = v.value.as_array;

	for(i = 0; i < indentation; i++) {
		fprintf(out, "\t");
	}

	switch(v.type) {
		case JSON_ARRAY:
			fprintf(out, "[\n");
			for(; a != NULL; a = a->next) {
				char *sub_terminator = ",\n";
				if(a->next == NULL)
					sub_terminator = "\n";

				pretty_print4(a->value,
					indentation + 1, sub_terminator, out);
			}
			for(i = 0; i < indentation; i++)
				fprintf(out, "\t");

			fprintf(out, "]%s", terminator);
			break;
		case JSON_OBJECT:
			fprintf(out, "{\n");
			for(; o != NULL; o = o->next) {
				char *sub_terminator = ",\n";

				for(i = 0; i < indentation + 1; i++)
					fprintf(out, "\t");

				if(o->next == NULL)
						sub_terminator = "\n";

				fprintf(out, "\"%s\" : \n", o->key);
				pretty_print4(o->value,
					indentation + 2, sub_terminator, out);
			}
			for(i = 0; i < indentation; i++)
				fprintf(out, "\t");

			fprintf(out, "}%s", terminator);
			break;
		case JSON_STRING:
		case JSON_INT:
		case JSON_FLOAT:
		case JSON_T:
		case JSON_F:
		case JSON_NIL:
			print_value3(v, terminator, out);
	}
}

static inline void pretty_print2(json_t v, int indentation)
{
	pretty_print4(v, indentation, "\n", stdout);
}

void pretty_print(json_t v)
{
	pretty_print2(v, 0);
}

/**
 * Copies a json_t to some space assigned by malloc.
 */
json_t *jsondup(json_t *json)
{
	json_t *space = (json_t *) malloc(sizeof(json_t));
	if(space == NULL) {
		perror("jsondup");
		return NULL;
	}

	return memcpy(space, json, sizeof(json_t));
}

json_t empty_array(void)
{
	struct json_value j;
	j.type = JSON_ARRAY;
	j.value.as_array = NULL;

	return j;
}

struct json_object *
append_to_object(struct json_object *pair, struct json_object *rest)
{
	pair->next = rest;
	return pair;
}

json_t create_object(struct json_object *members)
{
	struct json_value j;
	j.type = JSON_OBJECT;
	j.value.as_obj = members;
	return j;
}

json_t create_array(struct json_array *members)
{
	struct json_value j;
	j.type = JSON_ARRAY;
	j.value.as_array = members;
	return j;
}

struct json_object *create_pair(json_t key, json_t value)
{
	struct json_object *obj;

	assert(key.type == JSON_STRING);

	obj = (struct json_object*) malloc(sizeof(struct json_object));
	strcpy(obj->key, key.value.s);
	obj->value = value;
	obj->next = NULL;

	return obj;
}

struct json_array *create_array_value(json_t value)
{
	struct json_array *arr;

	arr = (struct json_array *) malloc(sizeof(struct json_array));
	arr->value = value;
	arr->next = NULL;

	return arr;
}

struct json_array *
append_to_array(struct json_array *fst, struct json_array *rest)
{
	fst->next = rest;
	return fst;
}


void json_free_inner();

void json_free_array(struct json_array *val)
{
	if(val == NULL)
		return;
	json_free_inner(val->value);
	if(val->next != NULL)
		json_free_array(val->next);
	free(val);
}

void json_free_object(struct json_object *val)
{
	if(val == NULL)
		return;
	json_free_inner(val->value);
	if(val->next != NULL)
		json_free_object(val->next);
	free(val);
}

/**
 * Deeply free a json value.
 * Then, it's free'd!!
 */
static inline void json_free2(json_t *val, int free_self)
{
	if(val == NULL)
		return;
	if(val->type == JSON_OBJECT && val->value.as_obj != NULL) {
		json_free_object(val->value.as_obj);
	}
	if(val->type == JSON_ARRAY && val->value.as_array != NULL) {
		json_free_array(val->value.as_array);
	}

	if(free_self)
		free(val);
}

void json_free(json_t *val)
{
	json_free2(val, 1);
}

void json_free_inner(json_t val)
{
	json_free2(&val, 0);
}



/** Getters / setters for structures **/

enum json_type json_get_type(json_t *val)
{
	return val->type;
}

json_t *json_object_get_value(json_t *val, stringl_t key)
{
	struct json_object *obj;
	if (val->type != JSON_OBJECT) {
		errno = EINVAL;
		return NULL;
	}

	for(obj = val->value.as_obj; obj != NULL; obj = obj->next) {
		if (!strcmp(obj->key, key)) {
			return &(obj->value);
		}
	}
	errno = ENOENT;
	return NULL;
}

json_t *json_array_index(json_t *val, int pos)
{
	struct json_array *arr;
	if (pos < 0) {
		errno = ERANGE;
		return NULL;
	}

	for(arr = val->value.as_array; arr != NULL && pos > 0; pos--, arr = arr->next);
	if (arr != NULL)
		return &(arr->value);

	errno = ENOENT;
	return NULL;
}

long json_array_len(json_t *val)
{
	long pos;
	struct json_array *arr;
	if (val->type != JSON_ARRAY) {
		errno = EINVAL;
		return -1;
	}

	for(pos = 0, arr = val->value.as_array; arr != NULL; pos++, arr = arr->next);

	return pos;
}

char *json_as_string(json_t *val)
{
	if (val->type != JSON_STRING) {
		errno = EINVAL;
		return NULL;
	}
	return val->value.s;
}

int json_as_bool(json_t *val)
{
	switch(val->type) {
		case JSON_T:
			return 1;
		case JSON_F:
			return 0;
		case JSON_NIL:
			return 0;
		default:
			errno = EINVAL;
			return -1;
	}
}

int json_is_null(json_t *val)
{
	return val->type == JSON_NIL;
}

long long json_as_int(json_t *val)
{
	switch(val->type) {
		case JSON_INT:
			return val->value.i;
		case JSON_FLOAT:
			return val->value.f;
		default:
			errno = EINVAL;
			return LLONG_MIN;
	}
}


int json_set_object(json_t *val, char *key, json_t *value)
{
	json_t key_j;
	key_j.type = JSON_STRING;
	strcpy(key_j.value.s, key);

	if(json_get_type(val) != JSON_OBJECT)
		return 0;
	if(val->value.as_obj == NULL) {
		val->value.as_obj = create_pair(key_j, *value);
		return 1;
	} else {
		struct json_object *cur;

		for(cur = val->value.as_obj; cur->next != NULL; cur = cur->next) {
			if(!strcmp(key, cur->key)) {
				cur->value = *value;
				return 1;
			}
		}
		if(!strcmp(key, cur->key)) {
			cur->value = *value;
			return 1;
		}
		cur->next = create_pair(key_j, *value);
		return 1;
	}
	return 0;
}



json_context_t json_context_init() {
	json_context_t r;
	r.ref = NULL;
	return r;
}

json_t *for_array_r(const json_t *val, json_context_t *ctx)
{
	if(val == NULL) {
		errno = EINVAL;
		return NULL;
	}
	if(ctx->ref == val) {
		if(ctx->arr->next != NULL) {
			ctx->arr = ctx->arr->next;
		} else {
			return NULL;
		}
	} else {
		ctx->ref = (json_t *) val;
		ctx->arr = val->value.as_array;
	}

	return &(ctx->arr->value);
}

json_t *for_object_r(const json_t *val, json_context_t *ctx, char **key)
{
	if(val == NULL) {
		errno = EINVAL;
		return NULL;
	}
	if(ctx->ref == val) {
		if(ctx->obj->next != NULL) {
			ctx->obj = ctx->obj->next;
		} else {
			return NULL;
		}
	} else {
		ctx->ref = (json_t *) val;
		ctx->obj = val->value.as_obj;
	}

	if (ctx->obj == NULL)
		return NULL;

	if (key != NULL) {
		*key = &(ctx->obj->key[0]);
	}

	return &(ctx->obj->value);
}


#define FOR_ARRAY(x, cur_pos, value, stmts) do {\
	struct json_array *arr; \
	for(cur_pos = 0, arr = x.value.as_array; arr != NULL; cur_pos++, arr = arr->next) { \
		value = arr->value; \
		stmts \
	} \
	return pos; \
} while(0)

#define FOR_OBJ(x, key, val, stmts) do {\
	struct json_object *obj; \
	for(obj = x.value.as_obj; obj != NULL; obj = obj->next) { \
		key = obj->key, val = obj->val; \
		stmts \
	} \
} while(0)
