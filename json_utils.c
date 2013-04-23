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

#include "headers.h"
#include "json_utils.h"


void print_value(json_t v)
{
	switch(v.type) {
		case JSON_STRING:
			printf("\"%s\"\n", v.value.s);
			break;
		case JSON_INT:
			printf("%lld\n", v.value.i);
			break;
		case JSON_FLOAT:
			printf("%lf\n", v.value.f);
			break;
		case JSON_ARRAY:
			printf("[Array]\n");
			break;
		case JSON_OBJECT:
			printf("[Object]\n");
			break;
		case JSON_T:
			printf("true\n");
			break;
		case JSON_F:
			printf("false\n");
			break;
		case JSON_NIL:
			printf("null\n");
			break;
	}
}

static inline void pretty_print2(json_t v, int indentation)
{
	int i;
	struct json_object *o = v.value.as_obj;
	struct json_array *a = v.value.as_array;

	for(i = 0; i < indentation; i++) {
		printf("\t");
	}

	switch(v.type) {
		case JSON_ARRAY:
			printf("[\n");
			for(; a != NULL; a = a->next) {
				pretty_print2(a->value,
					indentation + 1);
			}
			for(i = 0; i < indentation; i++)
				printf("\t");

			printf("]\n");
			break;
		case JSON_OBJECT:
			printf("{\n");
			for(; o != NULL; o = o->next) {
				for(i = 0; i < indentation + 1; i++)
					printf("\t");
				printf("\"%s\" : \n", o->key);
				pretty_print2(o->value,
					indentation + 2);
			}
			for(i = 0; i < indentation; i++)
				printf("\t");

			printf("}\n");
			break;
		case JSON_STRING:
		case JSON_INT:
		case JSON_FLOAT:
		case JSON_T:
		case JSON_F:
		case JSON_NIL:
			print_value(v);
	}
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
