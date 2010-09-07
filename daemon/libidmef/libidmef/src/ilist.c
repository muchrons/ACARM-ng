/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2005 Sandro Poppi.  All rights reserved

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: ilist.c,v 1.4 2009/02/15 14:15:04 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#include <idmef_types.h>
#include <idmef_global.h>
#include <string.h>


/*******************************************************************************
   Double linked list for IDMEFPtr. This creates a serial list of IDMEFPtr's.
   We need it to free unused IDMEFPtr created "on the fly" like in
   idmefp = newIDMEF_Message(
	      newIDMEF_Message_version("1.0", NULL),
	      newAlert(
		newAlert_messageid("1", NULL), NULL), NULL);
   as well as for searching and adding elements
*******************************************************************************/


/*===========================================================================
 * Name: libidmef_list_insert
 * Purpose: Insert a given IDMEF element below the parent. The element could
 *          also be a list but then the element has to be the head of it.
 *          Note: The function expects the parent's and elem's mutex be already
 *                locked!
 * Parameters:
 *           - parent : an IDMEFPtr to the parent
 *           - elem   : an IDMEFPtr to the element
 *
 * Returns: A pointer to the element, or NULL if error
 *
 *==========================================================================*/

IDMEFPtr libidmef_list_insert (IDMEFPtr parent, IDMEFPtr elem)
 {
  IDMEFPtr pnext, /* temporary var parent */
           p;     /* temporary pointer */
  
  if (!parent || !elem)
    return elem;
  
  if (elem->prev)
    /* elem is not the head of a list */
    return NULL;
  
#ifdef IDMEF_DEBUG
  printf ("Inserting %s after %s\n", elem->IDMEFXmlNodePtr->name,
                                   parent->IDMEFXmlNodePtr->name);
#endif

  if (!parent->next)
   {
    /* parent is the end of a list so simply append elem */
    parent->next = elem;
    elem->prev   = parent;
    return elem;
   }
   
  if (parent->next && !elem->next)
   {
    /* parent is not at the end of a list and elem is a single element */
    pnext        = parent->next;
    libidmef_idmefp_lock_op (pnext, IDMEF_Lock_Op_WRITE_LOCK);
    parent->next = elem;
    pnext->prev  = elem;
    elem->next   = pnext;
    elem->prev   = parent;
    libidmef_idmefp_lock_op (pnext, IDMEF_Lock_Op_WRITE_UNLOCK);
    return elem;
   }

  if (parent->next && elem->next)
   {
    /* parent is not at the end of a list and elem is head of a list */
    p = elem->next;
    
    /* find the end of the element list */
    while (p->next)
     {
      libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_READ_LOCK);
      p = p->next;
      libidmef_idmefp_lock_op (p->prev, IDMEF_Lock_Op_READ_UNLOCK);
     }
      
    pnext        = parent->next;
    libidmef_idmefp_lock_op (pnext, IDMEF_Lock_Op_WRITE_LOCK);
    libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_WRITE_LOCK);
    parent->next = elem;
    elem->prev   = parent;
    pnext->prev  = p;
    p->next      = pnext;
    libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_WRITE_UNLOCK);
    libidmef_idmefp_lock_op (pnext, IDMEF_Lock_Op_WRITE_UNLOCK);
    return elem;
   }
      
  /* if we come so far there has been an error */
  return NULL;
 }

/*===========================================================================
 * Name: libidmef_list_find
 * Purpose: Find a given IDMEF element by name below the parent.
 *          It's a replacement of getElement().
 * Parameters:
 *           - parent : an IDMEFPtr to the parent
 *           - name   : the element's name
 *
 * Returns: A pointer to the element, or NULL if error
 *
 *==========================================================================*/

IDMEFPtr libidmef_list_find (const IDMEFPtr parent, const char *name)
 {
  IDMEFPtr p;
  
  if (!parent || !name)
    return NULL;
    
  for (p = parent; p!= NULL; p = p->next)
   {
    libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_READ_LOCK);
    
    if (strcmp ((char*)p->IDMEFXmlNodePtr->children->name, name) == 0)
     {
      libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_READ_UNLOCK);
      return p;
     }
     
    libidmef_idmefp_lock_op (p, IDMEF_Lock_Op_READ_UNLOCK);
   }
  
  /* if we come so far the name was not found */
  return NULL;
 }
 
/*===========================================================================
 * Name: libidmef_list_add_element
 * Purpose: Add a child element to a parent.
 *          It's a replacement of addElement().
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

IDMEFPtr libidmef_list_add_element(IDMEFPtr parent, IDMEFPtr child)
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
 * Name: libidmef_list_add_elements
 * Purpose: Add a child elements to a parent.
 *          It's a replacement of addElements().
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

int libidmef_list_add_elements(IDMEFPtr parent, IDMEFPtr child, ...)
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
	if (!libidmef_list_add_element (pp, pc))
          break;

        count++;
        pp = pc; /* keep the order of our list as given in the parameters */
     }
  }
  va_end(ap);

  return count ;
}

/*===========================================================================
 * Name: libidmef_list_set_attribute
 * Purpose: Set attributes to an element.
 *          It's a replacement of setAttribute().
 * Parameters:
 *           - element : an IDMEFPtr to the element
 *           - attrib  : an IDMEFPtr to the attribute
 *
 * Returns: A pointer to the element, or NULL if error
 *==========================================================================*/

IDMEFPtr libidmef_list_set_attribute(IDMEFPtr element, IDMEFPtr attrib)
{
  xmlChar *propName = NULL;
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
 * Name: libidmef_list_set_attributes
 * Purpose: Set attributes to a element.
 *          It's a replacement of setAttributes().
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

int libidmef_list_set_attributes(IDMEFPtr element, IDMEFPtr attrib, ...)
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
  	if (!libidmef_list_set_attribute (element, p))
          break;

        count++;
     }
  }
  va_end(ap);

  return count;
}

/*==========================================================================*
 * Name: libidmef_list_has_element()
 * Purpose: Determines whether a parent element has a specified child element
 *          directly below it.  This is to say it only searches for first
 *          generation children.
 *          It's a replacement of hasElement().
 *
 * Parameters:
 *           - parent : an xmlNodePtr to the parent
 *           - name   : a string representing the child's name
 *
 * Returns: 1 for yes, 0 for no
 *==========================================================================*/

int libidmef_list_has_element(IDMEFPtr parent, char *name)
{
  xmlNodePtr p;

  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_LOCK);

  /* cycle through the children */
  for(p = parent->IDMEFXmlNodePtr->children; p != NULL; p = p->next)
  {
     if(strcmp((char*)p->name, name) == 0)
      {
       libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_UNLOCK);
       return 1;
      }
  }  
  libidmef_idmefp_lock_op (parent, IDMEF_Lock_Op_READ_UNLOCK);
  return 0;
}

/*===========================================================================
 * Name: libidmef_list_delete
 * Purpose: Delete an element from the list (non-recursive)
 * Parameters:
 *           - element : an IDMEFPtr to the element to be deleted
 *
 * Returns: 0 on error, 1 on success
 *==========================================================================*/

int libidmef_list_delete (IDMEFPtr element)
{
  if (!element)
    return 1;
  
  if (!element->prev && !element->next)
   {
    /* simple case: no list, single element */
    libidmef_idmefp_destroy (element, 0);
    return 1;
   }

  if (!element->prev && element->next)
   {
    /* element is head of a list */
    element->next->prev = NULL;
    element->next = NULL;
    libidmef_idmefp_destroy (element, 0);
    return 1;
   }
  
  if (element->prev && !element->next)
   {
    /* element is last element of a list */
    element->prev->next = NULL;
    element->prev = NULL;
    libidmef_idmefp_destroy (element, 0);
    return 1;
   }
  
  if (element->prev && element->next)
   {
    /* element is anywhere within a list */
    element->next->prev = element->prev;
    element->prev->next = element->next;
    element->next = NULL;
    element->prev = NULL;
    libidmef_idmefp_destroy (element, 0);
    return 1;
   }
  
  return 0;
}
