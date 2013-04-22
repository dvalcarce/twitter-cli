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

void print_value(json_t);
json_t empty_array(void);
json_t create_object(struct json_object *);
json_t create_array(struct json_array *);
struct json_object *create_pair(json_t, json_t);
struct json_object *append_to_object(struct json_object*, struct json_object*);

struct json_array *create_array_value(json_t);
struct json_array *append_to_array(struct json_array *, struct json_array *);
void json_free(json_t *);

#endif
