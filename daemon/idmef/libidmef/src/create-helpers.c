/*
Author: Adam C. Migus <adam@migus.org>
	Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2004 Adam C. Migus.  All rights reserved
Copyright (c) 2005 Adam C. Migus, Sandro Poppi.  All rights reserved

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: create-helpers.c,v 1.2 2005/07/10 09:38:51 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#include <idmef_types.h>
#include <idmef_global.h>
#include <idmef_list.h>
#include <string.h>

/*===========================================================================
 * Name: addElement
 * Purpose: Add a child element to a parent.
 * Parameters:
 *           - parent : an IDMEFPtr to the parent (mutex)
 *           - child  : an IDMEFPtr to the child (mutex)
 *
 * Returns: A pointer to the child, or NULL if error
 *
 * Note: We're not sure what type of error checking should occur here.  There's
 *       not really a way to ensure that the child should exist under the
 *       parent without implementing part of the DTD.  My initial thought is
 *       to let the DTD validation catch any errors at run time.
 *==========================================================================*/

IDMEFPtr addElement(IDMEFPtr parent, IDMEFPtr child)
{
  xmlNodePtr ret;
  
  if(!parent || !child || (parent->IDMEFXmlNodePtr->type != XML_ELEMENT_NODE))
    return NULL;

  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_WRITE_LOCK);
  libidmef_idmefp_lock_op (child, IDMEF_Lock_Op_READ_LOCK);
  ret = xmlAddChild (parent->IDMEFXmlNodePtr, child->IDMEFXmlNodePtr);
  
  if (ret)
  {
  	libidmef_list_insert (parent, child);
  }
  
  libidmef_idmefp_lock_op (child, IDMEF_Lock_Op_READ_UNLOCK);
  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_WRITE_UNLOCK);
  
  return ret ? child : NULL;
}

/*===========================================================================
 * Name: addElements
 * Purpose: Add a child elements to a parent.
 * Parameters:
 *           - parent : an IDMEFPtr to the parent
 *           - child  : an IDMEFPtr to the child
 *           - ...    : a variable size list of IDMEFPtr to children
 *
 * Returns: the number of children added
 *
 * NOTE: The last parameter MUST be NULL.  Otherwise, the function will not
 *       know when to stop reading in the variable length parameter list, and
 *       bad things will happen.
 *==========================================================================*/

int addElements(IDMEFPtr parent, IDMEFPtr child, ...)
{
  IDMEFPtr pp, pc;
  va_list ap;
  int count = 0;

  if(!parent || !child || (parent->IDMEFXmlNodePtr->type != XML_ELEMENT_NODE))
    return 0;
    
  /* cycle through the arguments */
  va_start(ap, child);
  pp = parent;
  pc = child;

  for(; pc != NULL; pc = va_arg(ap, IDMEFPtr))
  {
     if(pc->IDMEFXmlNodePtr->name != NULL)
     {
	if (!addElement (pp, pc))
          break;

        count++;
        pp = pc; /* keep the order of our list as given in the parameters */
     }
  }
  va_end(ap);

  return count ;
}

/*===========================================================================
 * Name: setAttribute
 * Purpose: Set attributes to an element.
 * Parameters:
 *           - element : an IDMEFPtr to the element
 *           - attrib  : an IDMEFPtr to the attribute
 *
 * Returns: A pointer to the element, or NULL if error
 *==========================================================================*/

IDMEFPtr setAttribute(IDMEFPtr element, IDMEFPtr attrib)
{
  char *propName = NULL;
  xmlChar *tmp_content = NULL;

  if(!element || !attrib)
    return NULL;

  libidmef_idmefp_lock_op (attrib, IDMEF_Lock_Op_WRITE_LOCK);
  propName = xmlStrdup(attrib->IDMEFXmlNodePtr->name);
  tmp_content = xmlGetProp(attrib->IDMEFXmlNodePtr, propName);
  libidmef_idmefp_lock_op (element, IDMEF_Lock_Op_WRITE_LOCK);
  
  if(xmlSetProp(element->IDMEFXmlNodePtr, propName, tmp_content))
  {
     libidmef_list_insert (element, attrib);
     libidmef_idmefp_lock_op (element, IDMEF_Lock_Op_WRITE_UNLOCK);
     libidmef_idmefp_lock_op (attrib, IDMEF_Lock_Op_WRITE_UNLOCK);
     
     if(tmp_content != NULL)
        xmlFree(tmp_content);
        
     if(propName != NULL)
       xmlFree(propName);
       
     return element;
  }

  if(tmp_content != NULL)
     xmlFree(tmp_content);

  if(propName != NULL)
     xmlFree(propName);

  libidmef_idmefp_lock_op (element, IDMEF_Lock_Op_WRITE_UNLOCK);
  libidmef_idmefp_lock_op (attrib, IDMEF_Lock_Op_WRITE_UNLOCK);
  return NULL;
}

/*===========================================================================
 * Name: setAttributes
 * Purpose: Set attributes to a element.
 * Parameters:
 *           - element : an IDMEFPtr to the element
 *           - attrib  : an IDMEFPtr to the attribute
 *           - ...     : a variable size list of IDMEFPtr's to attributes
 *
 * Returns: the number of attributes set
 *
 * NOTE: The last parameter MUST be NULL.  Otherwise, the function will not
 *       know when to stop reading in the variable length parameter list, and
 *       bad things will happen.
 *==========================================================================*/

int setAttributes(IDMEFPtr element, IDMEFPtr attrib, ...)
{
  IDMEFPtr p;
  va_list ap;
  int count = 0;

  if(!element || !attrib)
    return 0;

  /* cycle through the arguments */
  va_start(ap, attrib);
  p = attrib;

  for(; p != NULL; p = va_arg(ap, IDMEFPtr))
  {
     if(p->IDMEFXmlNodePtr->name != NULL)
     {
  	if (!setAttribute (element, p))
          break;

        count++;
     }
  }
  va_end(ap);

  return count;
}

/*==========================================================================*
 * Name: hasElement()
 * Purpose: Determines whether a parent element has a specified child element
 *          directly below it.  This is to say it only searches for first
 *          generation children.
 *
 * Parameters:
 *           - parent : an xmlNodePtr to the parent
 *           - name   : a string representing the child's name
 *
 * Returns: 1 for yes, 0 for no
 *==========================================================================*/

int hasElement(IDMEFPtr parent, char *name)
{
  xmlNodePtr p;

  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_LOCK);

  /* cycle through the children */
  for(p = parent->IDMEFXmlNodePtr->children; p != NULL; p = p->next)
  {
     if(strcmp(p->name, name) == 0)
      {
       libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_UNLOCK);
       return 1;
      }
  }  
  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_UNLOCK);
  return 0;
}

