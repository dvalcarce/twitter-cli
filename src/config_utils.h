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

#ifndef _CONFIG_UTILS_H
#define _CONFIG_UTILS_H

#include "headers.h"
#include "config_utils.h"

typedef struct config_json {
	json_t config_values;
	FILE *file;
} config_t;

int config_init(config_t *cfg, char *path);

int config_save(config_t *cfg);

int  config_save_key(config_t *cfg, char *key, char *element);

char *config_get_key(config_t *cfg, char *key);



#endif /* _CONFIG_UTILS_H */
