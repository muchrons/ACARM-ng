/*
Author: Adam C. Migus <adam@migus.org>

Copyright (c) 2004 Adam C. Migus.  All rights reserved

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: global.c,v 1.4 2009/02/15 14:15:04 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <idmef_types.h>
#include <idmef_global.h>
#include <string.h>

IDMEFPtr
libidmef_idmefp_create(IDMEF_Type type, xmlChar *name)
{
	IDMEFPtr idmefp = NULL;
	xmlNodePtr nodePtr = NULL;
	xmlDocPtr docPtr = NULL;

	if ((idmefp = (IDMEFPtr)malloc(sizeof(IDMEF))) == NULL)
		return (NULL);
	memset(idmefp, '\0', sizeof(IDMEF));

	if ((idmefp->IDMEFXmlMutexPtr = xmlNewMutex()) == NULL) {
		free (idmefp);
		return (NULL);
	}

	idmefp->IDMEF_type = type;

	switch (type) {
	case IDMEF_Type_ATTRIBUTE:
	case IDMEF_Type_CONTENT:
	case IDMEF_Type_NODE:
		if ((nodePtr = xmlNewNode(NULL, name)) == NULL)
			goto error_out;
		idmefp->IDMEFXmlNodePtr = nodePtr;
		break;
	case IDMEF_Type_DOC:
		if ((docPtr = xmlNewDoc((xmlChar*)XML_DEFAULT_VERSION)) == NULL)
			goto error_out;
  		xmlCreateIntSubset(docPtr,
			(xmlChar*)IDMEF_DTD_NAME,
			(xmlChar*)IDMEF_DTD_EXTERNAL_ID,
			(xmlChar*)IDMEF_DTD_PATH);
		if ((nodePtr = xmlNewNode(NULL, name)) == NULL)
			goto error_out;
		xmlDocSetRootElement(docPtr, nodePtr);
		idmefp->IDMEFXmlDocPtr = docPtr;
		idmefp->IDMEFXmlNodePtr = nodePtr;
		break;
	}
	return (idmefp);

error_out:
	switch (idmefp->IDMEF_type) {
	case IDMEF_Type_ATTRIBUTE:
	case IDMEF_Type_CONTENT:
	case IDMEF_Type_NODE:
		if (nodePtr)
			xmlFreeNode(nodePtr);
		break;
	case IDMEF_Type_DOC:
		if (docPtr)
			xmlFreeDoc(docPtr);
		break;
	}
	xmlFreeMutex(idmefp->IDMEFXmlMutexPtr);
	free (idmefp);
	return (NULL);
}

void
libidmef_idmefp_destroy(IDMEFPtr idmefp, int recursive)
{
	/* recursive = 0 means: Don't delete the node(s), not even the first */
        
	IDMEFPtr p, pprev;
        int isDoc = 0; /* flag to indicate deletion of doc node */
        
	if (!idmefp)
        	return;
                
        if (idmefp->prev)
        	/* when idmefp points to a node within a list let the list stop
                   at the previous element */
        	idmefp->prev->next = NULL;

        p = idmefp;
        
        while (p)
        {
	        switch (p->IDMEF_type) {
		case IDMEF_Type_ATTRIBUTE:
		case IDMEF_Type_CONTENT:
		case IDMEF_Type_NODE:
                	if (isDoc && p->IDMEF_type != IDMEF_Type_ATTRIBUTE)
                        /* when we previously deleted the doc there's
                           no node left to delete ;) */
                        	break;
                                
        		if (recursive == 1 &&
                            p == idmefp &&      /* only use xmlFreeNode once since
                                                  it's recursive for all children */
                            p->IDMEFXmlNodePtr)
                        {
				xmlUnlinkNode(p->IDMEFXmlNodePtr);
                                xmlFreeNode(p->IDMEFXmlNodePtr); /* recursive behaviour!!! */
                                p->IDMEFXmlNodePtr = NULL;
			}
                        break;
		case IDMEF_Type_DOC:
        		if (p->IDMEFXmlDocPtr)
				xmlFreeDoc(p->IDMEFXmlDocPtr);
                        isDoc = 1;
                        
                        /* set recursive to yes regardless what was set in the
                           call since this also deletes the nodes completely */
                        recursive = 1;
			break;
		}
	        if (p->IDMEFXmlMutexPtr)
                {
			xmlFreeMutex(p->IDMEFXmlMutexPtr);
                        p->IDMEFXmlMutexPtr = NULL;
                }
                        
                pprev = p;
                p = p->next;
		free (pprev);
                
                if (!recursive)
                        return;
	} /* while */
}

__inline void
libidmef_idmefp_lock_op(IDMEFPtr idmefp, IDMEF_Lock_Op lop)
{
	if (!idmefp)
        	return;
                
	switch (lop) {
	case IDMEF_Lock_Op_READ_LOCK:
	case IDMEF_Lock_Op_WRITE_LOCK:
		xmlMutexLock(idmefp->IDMEFXmlMutexPtr);
		break;
	case IDMEF_Lock_Op_READ_UNLOCK:
	case IDMEF_Lock_Op_WRITE_UNLOCK:
		xmlMutexUnlock(idmefp->IDMEFXmlMutexPtr);
		break;
	}
}

/*==========================================================================*
 * Name: libidmef_init()
 *
 * Purpose: Initialize global variables, and those structures we can reuse
 *          for each IDMEF Message.
 *
 * Parameters:
 *           none
 *
 * Returns: 1 for success, and 0 otherwise
 *
 * NOTE: You MUST call this function before you start to create IDMEF messages
 *==========================================================================*/
int libidmef_init (void)
 {
  
  xmlInitMemory();

  /* Initialize the validity context */

  vctxt = malloc(sizeof(xmlValidCtxt));

  if(vctxt == NULL)
  {
     fprintf(stderr,"global.c : libidmef_init() : malloc fail!\n");
     return 0;
  }
  vctxt->userData = (void *) stderr;
  vctxt->error = (xmlValidityErrorFunc) fprintf;
  vctxt->warning = (xmlValidityWarningFunc) fprintf;

  /* Initialize the dtd */

  dtd = xmlParseDTD(NULL,(xmlChar*)IDMEF_DTD_PATH);

  if(dtd == NULL)
  {
    fprintf(stderr,"global.c : libidmef_init(): problem parsing the DTD\n");
    return 0;
  }

  dtd->name = xmlStrdup((xmlChar*)IDMEF_DTD_NAME);

  return 1;
}
