/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2004 Sandro Poppi.  All rights reserved

Useful helper functions.

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_parser_helpers.h,v 1.2 2005/07/10 09:38:51 alcapone Exp $
*/

#ifndef _IDMEFXML_HELPERS_H
#define _IDMEFXML_HELPERS_H 1

#define MAX_ALERTID_BUFFER_SIZE 11   /* ulong (2^32 + 1) */

#include "idmef_types.h"

#define MAX_UTC_DATETIME_SIZE   23   /* YYYY-MM-DDThh:mm:ss.ssZ */
#define TZ_SIZE                 6    /* +hh:mm */
#define MAX_NTP_TIMESTAMP_SIZE  21   /* 0xNNNNNNNN.0xNNNNNNNN */

int is_heartbeat (IDMEFmessage *msg);
int is_alert (IDMEFmessage *msg);
unsigned long getStoredAlertID(char *);
int saveAlertID(unsigned long, char *);
char * intToString(int);
char * ulongToString(unsigned long);
xmlChar *timevalToNtpTimestamp(struct timeval *);
xmlChar *timevalToDatetime(struct timeval *);

#endif
