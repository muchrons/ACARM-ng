/*
Author: Sandro Poppi <spoppi@gmx.de>

Copyright (c) 2005 Sandro Poppi.  All rights reserved.

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_list.h,v 1.3 2005/07/17 11:57:30 alcapone Exp $
*/

#ifndef _IDMEF_LIST_H
#define _IDMEF_LIST_H 1

#include <idmef_types.h>

IDMEFPtr libidmef_list_insert (IDMEFPtr parent, IDMEFPtr elem);
IDMEFPtr libidmef_list_find (const IDMEFPtr parent, const char *name);
IDMEFPtr libidmef_list_add_element(IDMEFPtr, IDMEFPtr);
int libidmef_list_add_elements(IDMEFPtr, IDMEFPtr, ...);
IDMEFPtr libidmef_list_set_attribute(IDMEFPtr, IDMEFPtr);
int libidmef_list_set_attributes(IDMEFPtr, IDMEFPtr, ...);
int libidmef_list_has_element(IDMEFPtr, char *);
int libidmef_list_delete (IDMEFPtr element);

#endif
