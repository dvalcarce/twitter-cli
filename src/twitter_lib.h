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

#ifndef _TWITTER_LIB_H
#define _TWITTER_LIB_H


#define TWITTER_API_AUTHORIZE "https://api.twitter.com/oauth/authorize"
#define TWITTER_API_REQUEST_TOKEN "https://api.twitter.com/oauth/request_token"
#define TWITTER_API_ACCESS_TOKEN "https://api.twitter.com/oauth/access_token"

#define TWITTER_API_ACCOUNT_VERIFY_CREDENTIALS "https://api.twitter.com/1.1/account/verify_credentials.json"

#define TWITTER_API_STATUS_UPDATE "https://api.twitter.com/1.1/statuses/update.json"
#define TWITTER_API_STATUS_HOME_TIMELINE "https://api.twitter.com/1.1/statuses/home_timeline.json"


#include "twitter_keys.h"


int twitter_request_token(config_t *cfg);

int twitter_access_token(config_t *cfg, char *oauth_token, char *pin);

json_t *twitter_status_home_timeline(oauth_t oauth_token);

int twitter_account_verify_credentials(oauth_t oauth);

json_t *twitter_status_update(char *buffer, oauth_t oauth);

#endif /* _TWITTER_LIB_H */
