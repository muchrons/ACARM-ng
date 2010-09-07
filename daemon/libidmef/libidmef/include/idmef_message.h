/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2005 Sandro Poppi.  All rights reserved

Functions for IDMEFmessage handling

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_message.h,v 1.1 2005/07/17 11:51:52 alcapone Exp $
*/

#ifndef _IDMEFXML_MEMORY_H
#define _IDMEFXML_MEMORY_H 1

#include <libidmef/idmef_types.h>

int idmef_message_free		 (IDMEFmessage *msg);

#endif
