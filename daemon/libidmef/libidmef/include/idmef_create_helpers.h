/*
Author: Adam C. Migus <adam@migus.org>

Copyright (c) 2004 Adam C. Migus.  All rights reserved.

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_create_helpers.h,v 1.2 2005/07/10 09:38:51 alcapone Exp $
*/

#ifndef _IDMEF_CREATE_HELPERS_H
#define _IDMEF_CREATE_HELPERS_H 1

#define MAX_UTC_DATETIME_SIZE   23   /* YYYY-MM-DDThh:mm:ss.ssZ */
#define TZ_SIZE                 6    /* +hh:mm */
#define MAX_NTP_TIMESTAMP_SIZE  21   /* 0xNNNNNNNN.0xNNNNNNNN */

/* functions copied from idmefxml.c of libidmef 0.7.3 */
xmlNodePtr addElement(IDMEFPtr, IDMEFPtr);
int addElements(IDMEFPtr, IDMEFPtr, ...);
xmlNodePtr setAttribute(IDMEFPtr, IDMEFPtr);
int setAttributes(IDMEFPtr, IDMEFPtr, ...);
int hasElement(IDMEFPtr, char *);
xmlChar *timevalToNtpTimestamp(struct timeval *);
xmlChar *timevalToDatetime(struct timeval *);

/*==========================================================================*
 * Name: getElement()
 * Purpose: Retrieve a pointer to the first child element directly under a
 *          parent element whose name matches the provided string.
 * Parameters:
 *           - parent : an IDMEFPtr to the parent
 *           - name   : the element's name
 *
 * Returns: A pointer to the element, or NULL if error
 *
 * NOTE: This function has been replaced by libidmef_list_find(). Use that
 *      instead of getElement()
 *==========================================================================*/
#define getElement(parent,name) libidmef_list_find(parent,name)


#endif /* _IDMEF_CREATE_HELPERS_H */
