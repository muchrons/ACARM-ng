/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2005 Sandro Poppi.  All rights reserved

Functions for IDMEFmessage handling

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: message.c,v 1.1 2005/07/17 11:51:53 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "idmef_types.h"
#include "idmef_global.h"
#include "idmef_list.h"

/**************************************************
           helper functions
**************************************************/
int attlist_free (IDMEFattlist_global *attlist);
int name_free (IDMEFname *name);
int path_free (IDMEFpath *path);
int file_free (IDMEFfile *file);
int userid_free (IDMEFuserid *userid);
int command_free (IDMEFcommand *command);
int url_free (IDMEFurl *url);
int arg_free (IDMEFarg *arg);

/* Returns: 0 on error, 1 on success *
int _free ()
 {
  /* return 1 if NULL element *
  if (!)
    return 1;
 
  if (->attlist)
    if (!attlist_free (->attlist))
      return 0;
      
  if (->)
    free (->);
    
  if (->)
    if (!_free (->))
       return 0;
  
  
  if (->idmefp)
    libidmef_list_delete (->idmefp);
  
  free ();
  
  return 1;
 }
*/
/* Returns: 0 on error, 1 on success */
int key_free (IDMEFkey *key)
 {
  /* return 1 if NULL element */
  if (!key)
    return 1;
 
  if (key->key)
    free (key->key);
    
  if (key->idmefp)
    libidmef_list_delete (key->idmefp);
  
  free (key);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int value_free (IDMEFvalue *value)
 {
  /* return 1 if NULL element */
  if (!value)
    return 1;
 
  if (value->value)
    free (value->value);
    
  if (value->idmefp)
    libidmef_list_delete (value->idmefp);
  
  free (value);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int c_minor_device_free (IDMEFc_minor_device *c_minor_device)
 {
  /* return 1 if NULL element */
  if (!c_minor_device)
    return 1;
 
  if (c_minor_device->idmefp)
    libidmef_list_delete (c_minor_device->idmefp);
  
  free (c_minor_device);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int c_major_device_free (IDMEFc_major_device *c_major_device)
 {
  /* return 1 if NULL element */
  if (!c_major_device)
    return 1;
 
  if (c_major_device->idmefp)
    libidmef_list_delete (c_major_device->idmefp);
  
  free (c_major_device);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int minor_device_free (IDMEFminor_device *minor_device)
 {
  /* return 1 if NULL element */
  if (!minor_device)
    return 1;
 
  if (minor_device->idmefp)
    libidmef_list_delete (minor_device->idmefp);
  
  free (minor_device);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int major_device_free (IDMEFmajor_device *major_device)
 {
  /* return 1 if NULL element */
  if (!major_device)
    return 1;
 
  if (major_device->idmefp)
    libidmef_list_delete (major_device->idmefp);
  
  free (major_device);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int change_time_free (IDMEFchange_time *change_time)
 {
  /* return 1 if NULL element */
  if (!change_time)
    return 1;
 
  if (change_time->change_time)
    free (change_time->change_time);
    
  if (change_time->idmefp)
    libidmef_list_delete (change_time->idmefp);
  
  free (change_time);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int permission_free (IDMEFpermission *permission)
 {
  /* return 1 if NULL element */
  if (!permission)
    return 1;
 
  if (permission->permission)
    free (permission->permission);
    
  if (permission->idmefp)
    libidmef_list_delete (permission->idmefp);
  
  free (permission);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int contextEngineID_free (IDMEFcontextEngineID *contextEngineID)
 {
  /* return 1 if NULL element */
  if (!contextEngineID)
    return 1;
 
  if (contextEngineID->contextEngineID)
    free (contextEngineID->contextEngineID);
    
  if (contextEngineID->idmefp)
    libidmef_list_delete (contextEngineID->idmefp);
  
  free (contextEngineID);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int contextName_free (IDMEFcontextName *contextName)
 {
  /* return 1 if NULL element */
  if (!contextName)
    return 1;
 
  if (contextName->contextName)
    free (contextName->contextName);
    
  if (contextName->idmefp)
    libidmef_list_delete (contextName->idmefp);
  
  free (contextName);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int securityName_free (IDMEFsecurityName *securityName)
 {
  /* return 1 if NULL element */
  if (!securityName)
    return 1;
 
  if (securityName->securityName)
    free (securityName->securityName);
    
  if (securityName->idmefp)
    libidmef_list_delete (securityName->idmefp);
  
  free (securityName);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int community_free (IDMEFcommunity *community)
 {
  /* return 1 if NULL element */
  if (!community)
    return 1;
 
  if (community->community)
    free (community->community);
    
  if (community->idmefp)
    libidmef_list_delete (community->idmefp);
  
  free (community);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int oid_free (IDMEFoid *oid)
 {
  /* return 1 if NULL element */
  if (!oid)
    return 1;
 
  if (oid->oid)
    free (oid->oid);
    
  if (oid->idmefp)
    libidmef_list_delete (oid->idmefp);
  
  free (oid);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int http_method_free (IDMEFhttp_method *http_method)
 {
  /* return 1 if NULL element */
  if (!http_method)
    return 1;
 
  if (http_method->http_method)
    free (http_method->http_method);
    
  if (http_method->idmefp)
    libidmef_list_delete (http_method->idmefp);
  
  free (http_method);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int cgi_free (IDMEFcgi *cgi)
 {
  /* return 1 if NULL element */
  if (!cgi)
    return 1;
 
  if (cgi->cgi)
    free (cgi->cgi);
    
  if (cgi->idmefp)
    libidmef_list_delete (cgi->idmefp);
  
  free (cgi);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int number_free (IDMEFnumber *number)
 {
  /* return 1 if NULL element */
  if (!number)
    return 1;
 
  if (number->idmefp)
    libidmef_list_delete (number->idmefp);
  
  free (number);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int checksum_free (IDMEFchecksum *checksum)
 {
  /* return 1 if NULL element */
  if (!checksum)
    return 1;
 
  if (checksum->attlist)
    if (!attlist_free (checksum->attlist))
      return 0;

  if (checksum->value)
    if (!value_free (checksum->value))
       return 0;

  if (checksum->key)
    if (!key_free (checksum->key))
       return 0;
  
  if (checksum->idmefp)
    libidmef_list_delete (checksum->idmefp);
  
  free (checksum);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int inode_free (IDMEFinode *inode)
 {
  /* return 1 if NULL element */
  if (!inode)
    return 1;
 
  if (inode->attlist)
    if (!attlist_free (inode->attlist))
      return 0;
      
  if (inode->change_time)
    if (!change_time_free (inode->change_time))
       return 0;
      
  if (inode->number)
    if (!number_free (inode->number))
       return 0;
      
  if (inode->major_device)
    if (!major_device_free (inode->major_device))
       return 0;
      
  if (inode->minor_device)
    if (!minor_device_free (inode->minor_device))
       return 0;
      
  if (inode->c_major_device)
    if (!c_major_device_free (inode->c_major_device))
       return 0;
      
  if (inode->c_minor_device)
    if (!c_minor_device_free (inode->c_minor_device))
       return 0;
  
  if (inode->idmefp)
    libidmef_list_delete (inode->idmefp);
  
  free (inode);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int linkage_free (IDMEFlinkage *linkage)
 {
  /* return 1 if NULL element */
  if (!linkage)
    return 1;
 
  if (linkage->attlist)
    if (!attlist_free (linkage->attlist))
      return 0;
      
  if (linkage->name)
    if (!name_free (linkage->name))
       return 0;
  
  if (linkage->path)
    if (!path_free (linkage->path))
       return 0;
  
  if (linkage->file)
    if (!file_free (linkage->file))
       return 0;
  
  
  if (linkage->idmefp)
    libidmef_list_delete (linkage->idmefp);
  
  free (linkage);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int fileaccess_free (IDMEFfileaccess *fileaccess)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!fileaccess)
    return 1;
 
  if (fileaccess->attlist)
    if (!attlist_free (fileaccess->attlist))
      return 0;
      
  if (fileaccess->userid)
    if (!userid_free (fileaccess->userid))
       return 0;
  
  for (i = 0; i < fileaccess->npermissions; i++)
    if (fileaccess->permissions && fileaccess->permissions[i])
      if (!permission_free (fileaccess->permissions[i]))
         return 0;
  
  if (fileaccess->idmefp)
    libidmef_list_delete (fileaccess->idmefp);
  
  free (fileaccess);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int disk_size_free (IDMEFdisk_size *disk_size)
 {
  /* return 1 if NULL element */
  if (!disk_size)
    return 1;
  
  if (disk_size->idmefp)
    libidmef_list_delete (disk_size->idmefp);
  
  free (disk_size);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int data_size_free (IDMEFdata_size *data_size)
 {
  /* return 1 if NULL element */
  if (!data_size)
    return 1;
 
  if (data_size->idmefp)
    libidmef_list_delete (data_size->idmefp);
  
  free (data_size);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int access_time_free (IDMEFaccess_time *access_time)
 {
  /* return 1 if NULL element */
  if (!access_time)
    return 1;
 
  if (access_time->access_time)
    free (access_time->access_time);
    
  if (access_time->idmefp)
    libidmef_list_delete (access_time->idmefp);
  
  free (access_time);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int modify_time_free (IDMEFmodify_time *modify_time)
 {
  /* return 1 if NULL element */
  if (!modify_time)
    return 1;
 
  if (modify_time->modify_time)
    free (modify_time->modify_time);
    
  if (modify_time->idmefp)
    libidmef_list_delete (modify_time->idmefp);
  
  free (modify_time);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int create_time_free (IDMEFcreate_time *create_time)
 {
  /* return 1 if NULL element */
  if (!create_time)
    return 1;
 
  if (create_time->create_time)
    free (create_time->create_time);

  if (create_time->idmefp)
    libidmef_list_delete (create_time->idmefp);
  
  free (create_time);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int snmpservice_free (IDMEFsnmpservice *snmpservice)
 {
  /* return 1 if NULL element */
  if (!snmpservice)
    return 1;
 
  if (snmpservice->attlist)
    if (!attlist_free (snmpservice->attlist))
      return 0;

  if (snmpservice->oid)
    if (!oid_free (snmpservice->oid))
       return 0;
  
  if (snmpservice->community)
    if (!community_free (snmpservice->community))
       return 0;
  
  if (snmpservice->securityName)
    if (!securityName_free (snmpservice->securityName))
       return 0;
  
  if (snmpservice->contextName)
    if (!contextName_free (snmpservice->contextName))
       return 0;
  
  if (snmpservice->contextEngineID)
    if (!contextEngineID_free (snmpservice->contextEngineID))
       return 0;
  
  if (snmpservice->command)
    if (!command_free (snmpservice->command))
       return 0;
  
  if (snmpservice->idmefp)
    libidmef_list_delete (snmpservice->idmefp);
  
  free (snmpservice);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int webservice_free (IDMEFwebservice *webservice)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!webservice)
    return 1;
 
  if (webservice->attlist)
    if (!attlist_free (webservice->attlist))
      return 0;
      
  if (webservice->url)
    if (!url_free (webservice->url))
       return 0;
  
  if (webservice->cgi)
    if (!cgi_free (webservice->cgi))
       return 0;
  
  if (webservice->http_method)
    if (!http_method_free (webservice->http_method))
       return 0;
  
  for (i = 0; i < webservice->nargs; i++)
    if (webservice->args && webservice->args[i])
      if (!arg_free (webservice->args[i]))
         return 0;
  
  if (webservice->idmefp)
    libidmef_list_delete (webservice->idmefp);
  
  free (webservice);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int protocol_free (IDMEFprotocol *protocol)
 {
  /* return 1 if NULL element */
  if (!protocol)
    return 1;
 
  if (protocol->protocol)
    free (protocol->protocol);
    
  if (protocol->idmefp)
    libidmef_list_delete (protocol->idmefp);
  
  free (protocol);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int portlist_free (IDMEFportlist *portlist)
 {
  /* return 1 if NULL element */
  if (!portlist)
    return 1;
 
  if (portlist->portlist)
    free (portlist->portlist);
    
  if (portlist->idmefp)
    libidmef_list_delete (portlist->idmefp);
  
  free (portlist);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int port_free (IDMEFport *port)
 {
  /* return 1 if NULL element */
  if (!port)
    return 1;
 
  if (port->idmefp)
    libidmef_list_delete (port->idmefp);
  
  free (port);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int userid_free (IDMEFuserid *userid)
 {
  /* return 1 if NULL element */
  if (!userid)
    return 1;
 
  if (userid->attlist)
    if (!attlist_free (userid->attlist))
      return 0;
      
  if (userid->ident)
    free (userid->ident);
    
  if (userid->tty)
    free (userid->tty);
    
  if (userid->name)
    if (!name_free (userid->name))
       return 0;
  
  if (userid->number)
    if (!number_free (userid->number))
       return 0;
  
  if (userid->idmefp)
    libidmef_list_delete (userid->idmefp);
  
  free (userid);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int url_free (IDMEFurl *url)
 {
  /* return 1 if NULL element */
  if (!url)
    return 1;
 
  if (url->url)
    free (url->url);
    
  if (url->idmefp)
    libidmef_list_delete (url->idmefp);
  
  free (url);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int netmask_free (IDMEFnetmask *netmask)
 {
  /* return 1 if NULL element */
  if (!netmask)
    return 1;
 
  if (netmask->netmask)
    free (netmask->netmask);
    
  if (netmask->idmefp)
    libidmef_list_delete (netmask->idmefp);
  
  free (netmask);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int buffer_free (IDMEFbuffer *buffer)
 {
  /* return 1 if NULL element */
  if (!buffer)
    return 1;
 
  if (buffer->buffer)
    free (buffer->buffer);

  if (buffer->idmefp)
    libidmef_list_delete (buffer->idmefp);
  
  free (buffer);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int size_free (IDMEFsize *size)
 {
  /* return 1 if NULL element */
  if (!size)
    return 1;
 
  if (size->idmefp)
    libidmef_list_delete (size->idmefp);
  
  free (size);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int program_free (IDMEFprogram *program)
 {
  /* return 1 if NULL element */
  if (!program)
    return 1;
 
  if (program->program)
    free (program->program);
    
  if (program->idmefp)
    libidmef_list_delete (program->idmefp);
  
  free (program);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int alertident_free (IDMEFalertident *alertident)
 {
  /* return 1 if NULL element */
  if (!alertident)
    return 1;
 
  if (alertident->attlist)
    if (!attlist_free (alertident->attlist))
      return 0;
      
  if (alertident->data)
    free (alertident->data);
    
  if (alertident->analyzerid)
    free (alertident->analyzerid);
    
  if (alertident->idmefp)
    libidmef_list_delete (alertident->idmefp);
  
  free (alertident);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int command_free (IDMEFcommand *command)
 {
  /* return 1 if NULL element */
  if (!command)
    return 1;
 
  if (command->command)
    free (command->command);
    
  if (command->idmefp)
    libidmef_list_delete (command->idmefp);
  
  free (command);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int confidence_free (IDMEFconfidence *confidence)
 {
  /* return 1 if NULL element */
  if (!confidence)
    return 1;
 
  if (confidence->attlist)
    if (!attlist_free (confidence->attlist))
      return 0;
      
  if (confidence->idmefp)
    libidmef_list_delete (confidence->idmefp);
  
  free (confidence);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int action_free (IDMEFaction *action)
 {
  /* return 1 if NULL element */
  if (!action)
    return 1;
 
  if (action->attlist)
    if (!attlist_free (action->attlist))
      return 0;
      
  if (action->data)
    free (action->data);
  
  if (action->idmefp)
    libidmef_list_delete (action->idmefp);
  
  free (action);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int impact_free (IDMEFimpact *impact)
 {
  /* return 1 if NULL element */
  if (!impact)
    return 1;
 
  if (impact->attlist)
    if (!attlist_free (impact->attlist))
      return 0;
      
  if (impact->data)
    free (impact->data);
    
  if (impact->idmefp)
    libidmef_list_delete (impact->idmefp);
  
  free (impact);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int file_free (IDMEFfile *file)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!file)
    return 1;
 
  if (file->attlist)
    if (!attlist_free (file->attlist))
      return 0;
      
  if (file->ident)
    free (file->ident);
    
  if (file->name)
    if (!name_free (file->name))
       return 0;
  
  if (file->path)
    if (!path_free (file->path))
       return 0;
  
  if (file->create_time)
    if (!create_time_free (file->create_time))
       return 0;
  
  if (file->modify_time)
    if (!modify_time_free (file->modify_time))
       return 0;
  
  if (file->access_time)
    if (!access_time_free (file->access_time))
       return 0;
  
  if (file->data_size)
    if (!data_size_free (file->data_size))
       return 0;
  
  if (file->disk_size)
    if (!disk_size_free (file->disk_size))
       return 0;
  
  for (i = 0; i < file->nfileaccesses; i++)
    if (file->fileaccesses && file->fileaccesses[i])
      if (!fileaccess_free (file->fileaccesses[i]))
         return 0;
  
  for (i = 0; i < file->nlinkages; i++)
    if (file->linkages && file->linkages[i])
      if (!linkage_free (file->linkages[i]))
         return 0;
  
  if (file->inode)
    if (!inode_free (file->inode))
       return 0;
  
  for (i = 0; i < file->nchecksums; i++)
    if (file->checksums && file->checksums[i])
      if (!checksum_free (file->checksums[i]))
         return 0;
  
  if (file->file_type)
    free (file->file_type);
    
  if (file->idmefp)
    libidmef_list_delete (file->idmefp);
  
  free (file);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int service_free (IDMEFservice *service)
 {
  /* return 1 if NULL element */
  if (!service)
    return 1;
 
  if (service->attlist)
    if (!attlist_free (service->attlist))
      return 0;
      
  if (service->ident)
    free (service->ident);
    
  if (service->iana_protocol_name)
    free (service->iana_protocol_name);
    
  if (service->name)
    if (!name_free (service->name))
       return 0;
  
  if (service->port)
    if (!port_free (service->port))
       return 0;
  
  if (service->portlist)
    if (!portlist_free (service->portlist))
       return 0;
  
  if (service->protocol)
    if (!protocol_free (service->protocol))
       return 0;
  
  if (service->webservice)
    if (!webservice_free (service->webservice))
       return 0;
  
  if (service->snmpservice)
    if (!snmpservice_free (service->snmpservice))
       return 0;
  
  if (service->idmefp)
    libidmef_list_delete (service->idmefp);
  
  free (service);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int user_free (IDMEFuser *user)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!user)
    return 1;
 
  if (user->attlist)
    if (!attlist_free (user->attlist))
      return 0;
      
  if (user->ident)
    free (user->ident);
    
  for (i = 0; i < user->nuserids; i++)
    if (user->userids && user->userids[i])
      if (!userid_free (user->userids[i]))
         return 0;
  
  if (user->idmefp)
    libidmef_list_delete (user->idmefp);
  
  free (user);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int reference_free (IDMEFreference *reference)
 {
  /* return 1 if NULL element */
  if (!reference)
    return 1;
 
  if (reference->meaning)
    free (reference->meaning);
    
  if (reference->name)
    if (!name_free (reference->name))
       return 0;
  
  if (reference->url)
    if (!url_free (reference->url))
       return 0;
  
  if (reference->idmefp)
    libidmef_list_delete (reference->idmefp);
  
  free (reference);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int env_free (IDMEFenv *env)
 {
  /* return 1 if NULL element */
  if (!env)
    return 1;
 
  if (env->env)
    free (env->env);
  
  if (env->idmefp)
    libidmef_list_delete (env->idmefp);
  
  free (env);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int arg_free (IDMEFarg *arg)
 {
  /* return 1 if NULL element */
  if (!arg)
    return 1;
 
  if (arg->arg)
    free (arg->arg);
    
  if (arg->idmefp)
    libidmef_list_delete (arg->idmefp);
  
  free (arg);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int path_free (IDMEFpath *path)
 {
  /* return 1 if NULL element */
  if (!path)
    return 1;
 
  if (path->path)
    free (path->path);
    
  if (path->idmefp)
    libidmef_list_delete (path->idmefp);
  
  free (path);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int pid_free (IDMEFpid *pid)
 {
  /* return 1 if NULL element */
  if (!pid)
    return 1;
 
  if (pid->idmefp)
    libidmef_list_delete (pid->idmefp);
  
  free (pid);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int addressaddress_free (IDMEFaddressaddress *address)
 {
  /* return 1 if NULL element */
  if (!address)
    return 1;
 
  if (address->address)
    free (address->address);
    
  if (address->idmefp)
    libidmef_list_delete (address->idmefp);
  
  free (address);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int address_free (IDMEFaddress *address)
 {
  /* return 1 if NULL element */
  if (!address)
    return 1;
 
  if (address->attlist)
    if (!attlist_free (address->attlist))
      return 0;
      
  if (address->ident)
    free (address->ident);
    
  if (address->vlan_name)
    free (address->vlan_name);
    
  if (address->address)
    if (!addressaddress_free (address->address))
       return 0;
  
  if (address->netmask)
    if (!netmask_free (address->netmask))
       return 0;
  
  if (address->idmefp)
    libidmef_list_delete (address->idmefp);
  
  free (address);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int name_free (IDMEFname *name)
 {
  /* return 1 if NULL element */
  if (!name)
    return 1;
 
  if (name->name)
    free (name->name);
    
  if (name->idmefp)
    libidmef_list_delete (name->idmefp);
  
  free (name);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int location_free (IDMEFlocation *location)
 {
  /* return 1 if NULL element */
  if (!location)
    return 1;
 
  if (location->location)
    free (location->location);
    
  if (location->idmefp)
    libidmef_list_delete (location->idmefp);
  
  free (location);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int process_free (IDMEFprocess *process)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!process)
    return 1;
 
  if (process->attlist)
    if (!attlist_free (process->attlist))
      return 0;
      
  if (process->ident)
    free (process->ident);
    
  if (process->name)
    if (!name_free (process->name))
       return 0;
  
  if (process->pid)
    if (!pid_free (process->pid))
       return 0;
  
  if (process->path)
    if (!path_free (process->path))
       return 0;
  
  for (i = 0; i < process->nargs; i++)
    if (process->args && process->args[i])
      if (!arg_free (process->args[i]))
         return 0;
  
  for (i = 0; i < process->nenvs; i++)
    if (process->envs &&process->envs[i] )
      if (!env_free (process->envs[i]))
         return 0;
  
  if (process->idmefp)
    libidmef_list_delete (process->idmefp);
  
  free (process);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int node_free (IDMEFnode *node)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!node)
    return 1;
 
  if (node->attlist)
    if (!attlist_free (node->attlist))
      return 0;
      
  if (node->ident)
    free (node->ident);
    
  if (node->location)
    if (!location_free (node->location))
       return 0;
  
  if (node->name)
    if (!name_free (node->name))
       return 0;
  
  for (i = 0; i < node->naddresses; i++)
    if (node->addresses && node->addresses[i])
      if (!address_free (node->addresses[i]))
         return 0;
  
  if (node->idmefp)
    libidmef_list_delete (node->idmefp);
  
  free (node);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int heartbeatinterval_free (IDMEFheartbeatinterval *heartbeatinterval)
 {
  /* return 1 if NULL element */
  if (!heartbeatinterval)
    return 1;
 
  if (heartbeatinterval->attlist)
    if (!attlist_free (heartbeatinterval->attlist))
      return 0;
  
  if (heartbeatinterval->idmefp)
    libidmef_list_delete (heartbeatinterval->idmefp);
  
  free (heartbeatinterval);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int correlationalert_free (IDMEFcorrelationalert *correlationalert)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!correlationalert)
    return 1;
 
  if (correlationalert->attlist)
    if (!attlist_free (correlationalert->attlist))
      return 0;
      
  if (correlationalert->name)
    if (!name_free (correlationalert->name))
       return 0;
  
  for (i = 0; i < correlationalert->nalertidents; i++)
    if (correlationalert->alertidents && correlationalert->alertidents[i])
      if (!alertident_free (correlationalert->alertidents[i]))
         return 0;
  
  if (correlationalert->idmefp)
    libidmef_list_delete (correlationalert->idmefp);
  
  free (correlationalert);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int overflowalert_free (IDMEFoverflowalert *overflowalert)
 {
  /* return 1 if NULL element */
  if (!overflowalert)
    return 1;
 
  if (overflowalert->attlist)
    if (!attlist_free (overflowalert->attlist))
      return 0;
      
  if (overflowalert->program)
    if (!program_free (overflowalert->program))
       return 0;
  
  if (overflowalert->size)
    if (!size_free (overflowalert->size))
       return 0;
  
  if (overflowalert->buffer)
    if (!buffer_free (overflowalert->buffer))
       return 0;
  
  if (overflowalert->idmefp)
    libidmef_list_delete (overflowalert->idmefp);
  
  free (overflowalert);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int toolalert_free (IDMEFtoolalert *toolalert)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!toolalert)
    return 1;
 
  if (toolalert->attlist)
    if (!attlist_free (toolalert->attlist))
      return 0;
      
  if (toolalert->name)
    if (!name_free (toolalert->name))
       return 0;

  if (toolalert->command)
    if (!command_free (toolalert->command))
       return 0;
  
  for (i = 0; i < toolalert->nalertidents; i++)
    if (toolalert->alertidents && toolalert->alertidents[i])
      if (!alertident_free (toolalert->alertidents[i]))
         return 0;
  
  if (toolalert->idmefp)
    libidmef_list_delete (toolalert->idmefp);
  
  free (toolalert);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int additionaldata_free (IDMEFadditionaldata *additionaldata)
 {
  /* return 1 if NULL element */
  if (!additionaldata)
    return 1;
 
  if (additionaldata->attlist)
    if (!attlist_free (additionaldata->attlist))
      return 0;
      
  if (additionaldata->meaning)
    free (additionaldata->meaning);
    
  if (additionaldata->data)
    free (additionaldata->data);
  
  if (additionaldata->idmefp)
    libidmef_list_delete (additionaldata->idmefp);
  
  free (additionaldata);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int assessment_free (IDMEFassessment *assessment)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!assessment)
    return 1;
 
  if (assessment->attlist)
    if (!attlist_free (assessment->attlist))
      return 0;
  
  if (assessment->impact)
    if (!impact_free (assessment->impact))
       return 0;
  
  for (i = 0; i < assessment->nactions; i++)
    if (assessment->actions && assessment->actions[i])
      if (!action_free (assessment->actions[i]))
         return 0;
  
  if (assessment->confidence)
    if (!confidence_free (assessment->confidence))
       return 0;
  
  if (assessment->idmefp)
    libidmef_list_delete (assessment->idmefp);
  
  free (assessment);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int target_free (IDMEFtarget *target)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!target)
    return 1;
 
  if (target->attlist)
    if (!attlist_free (target->attlist))
      return 0;
      
  if (target->ident)
    free (target->ident);
  
  if (target->interface)
    free (target->interface);
  
  if (target->node)
    if (!node_free (target->node))
      return 0;
  
  if (target->user)
    if (!user_free (target->user))
      return 0;
  
  if (target->process)
    if (!process_free (target->process))
      return 0;
  
  if (target->service)
    if (!service_free (target->service))
      return 0;
  
  for (i = 0; i < target->nfiles; i++)
    if (target->files && target->files[i])
      if (!file_free (target->files[i]))
        return 0;
  
  if (target->idmefp)
    libidmef_list_delete (target->idmefp);
  
  free (target);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int source_free (IDMEFsource *source)
 {
  /* return 1 if NULL element */
  if (!source)
    return 1;
 
  if (source->attlist)
    if (!attlist_free (source->attlist))
      return 0;
  
  if (source->ident)
    free (source->ident);    
  
  if (source->interface)
    free (source->interface);    
  
  if (source->node)
    if (!node_free (source->node))
      return 0;
  
  if (source->user)
    if (!user_free (source->user))
      return 0;
  
  if (source->process)
    if (!process_free (source->process))
      return 0;
  
  if (source->service)
    if (!service_free (source->service))
      return 0;
  
  if (source->idmefp)
    libidmef_list_delete (source->idmefp);
  
  free (source);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int classification_free (IDMEFclassification *classification)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!classification)
    return 1;
 
  if (classification->ident)
    free (classification->ident);
    
  if (classification->text)
    free (classification->text);
    
  for (i = 0; i < classification->nreferences; i++)
    if (classification->references && classification->references[i])
      if (!reference_free (classification->references[i]))
        return 0;
  
  if (classification->idmefp)
    libidmef_list_delete (classification->idmefp);
  
  free (classification);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int time_free (IDMEFtime *time)
 {
  /* return 1 if NULL element */
  if (!time)
    return 1;
 
  if (time->attlist)
    if (!attlist_free (time->attlist))
      return 0;
  
  if (time->ntpstamp)
    free (time->ntpstamp);    
  
  if (time->string)
    free (time->string);    
  
  /* Note: struct timeval tv not (yet) used in parser.c */
  
  if (time->idmefp)
    libidmef_list_delete (time->idmefp);
  
  free (time);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int analyzer_free (IDMEFanalyzer *analyzer)
 {
  /* return 1 if NULL element */
  if (!analyzer)
    return 1;
 
  if (analyzer->attlist)
    if (!attlist_free (analyzer->attlist))
      return 0;
      
  if (analyzer->analyzerid)
    free (analyzer->analyzerid);
  
  if (analyzer->name)
    free (analyzer->name);
  
  if (analyzer->manufacturer)
    free (analyzer->manufacturer);
  
  if (analyzer->model)
    free (analyzer->model);
  
  if (analyzer->version)
    free (analyzer->version);
  
  if (analyzer->analyzer_class)
    free (analyzer->analyzer_class);
  
  if (analyzer->ostype)
    free (analyzer->ostype);
  
  if (analyzer->osversion)
    free (analyzer->osversion);
    
  if (analyzer->node)
    if (!node_free (analyzer->node))
      return 0;
  
  if (analyzer->process)
    if (!process_free (analyzer->process))
      return 0;
  
  if (analyzer->analyzer)
    if (!analyzer_free (analyzer->analyzer))
      return 0;
  
  if (analyzer->idmefp)
    libidmef_list_delete (analyzer->idmefp);
  
  free (analyzer);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int heartbeat_free (IDMEFheartbeat *heartbeat)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!heartbeat)
    return 1;
 
  if (heartbeat->attlist)
    if (!attlist_free (heartbeat->attlist))
      return 0;
      
  if (heartbeat->messageid)
    free (heartbeat->messageid);
  
  if (heartbeat->analyzer)
    if (!analyzer_free (heartbeat->analyzer))
      return 0;
  
  if (heartbeat->createtime)
    if (!time_free (heartbeat->createtime))
      return 0;
  
  if (heartbeat->analyzertime)
    if (!time_free (heartbeat->analyzertime))
      return 0;
  
  if (heartbeat->heartbeatinterval)
    if (!heartbeatinterval_free (heartbeat->heartbeatinterval))
      return 0;
      
  for (i = 0; i < heartbeat->nadditionaldatas; i++)
    if (heartbeat->additionaldatas && heartbeat->additionaldatas[i])
      if (!additionaldata_free (heartbeat->additionaldatas[i]))
        return 0;
  
  if (heartbeat->idmefp)
    libidmef_list_delete (heartbeat->idmefp);
  
  free (heartbeat);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int alert_free (IDMEFalert *alert)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!alert)
    return 1;
 
  if (alert->attlist)
    if (!attlist_free (alert->attlist))
      return 0;
      
  if (alert->messageid)
    free (alert->messageid);
    
  if (alert->analyzer)
    if (!analyzer_free (alert->analyzer))
      return 0;
  
  if (alert->createtime)
    if (!time_free (alert->createtime))
      return 0;
  
  if (alert->classification)
    if (!classification_free (alert->classification))
      return 0;
  
  if (alert->detecttime)
    if (!time_free (alert->detecttime))
      return 0;
  
  if (alert->analyzertime)
    if (!time_free (alert->analyzertime))
      return 0;
  
  for (i = 0; i < alert->nsources; i++)
    if (alert->sources && alert->sources[i])
      if (!source_free (alert->sources[i]))
        return 0;
  
  for (i = 0; i < alert->ntargets; i++)
    if (alert->targets && alert->targets[i])
      if (!target_free (alert->targets[i]))
        return 0;
  
  if (alert->assessment)
    if (!assessment_free (alert->assessment))
      return 0;
  
  for (i = 0; i < alert->nadditionaldatas; i++)
    if (alert->additionaldatas && alert->additionaldatas[i])
      if (!additionaldata_free (alert->additionaldatas[i]))
        return 0;
  
  if (alert->toolalert)
    if (!toolalert_free (alert->toolalert))
      return 0;
  
  if (alert->overflowalert)
    if (!overflowalert_free (alert->overflowalert))
      return 0;
  
  if (alert->correlationalert)
    if (!correlationalert_free (alert->correlationalert))
      return 0;
  
  if (alert->idmefp)
    libidmef_list_delete (alert->idmefp);
  
  free (alert);
  
  return 1;
 }

/* Returns: 0 on error, 1 on success */
int attlist_free (IDMEFattlist_global *attlist)
 {
  /* return 1 if NULL element */
  if (!attlist)
    return 1;
 
  if (attlist->xmlns)
    free (attlist->xmlns);
    
  if (attlist->xmlns_idmef)
    free (attlist->xmlns_idmef);
  
  if (attlist->xml_lang)
    free (attlist->xml_lang);
    
  if (attlist->xml_space)
    free (attlist->xml_space);
    
  free (attlist);
  
  return 1;
 }

/**************************************************
           API
**************************************************/

/**
 * idmef_message_free
 * @msg: A pointer to an IDMEFmessage object to be freed
 *
 * Frees all memory of an IDMEFmessage object
 *
 * Returns: 0 on error, 1 on success
 */
int idmef_message_free (IDMEFmessage *msg)
 {
  int i;
  
  /* return 1 if NULL element */
  if (!msg)
    return 1;
    
  /* now run through the elements */
  if (msg->attlist)
    if (!attlist_free (msg->attlist))
      return 0;
      
  if (msg->version)
    free (msg->version);
    
  for (i = 0; i < msg->nalerts; i++)
    if (msg->alerts && msg->alerts[i])
      if (alert_free (msg->alerts[i]))
        return 0;
  
  for (i = 0; i < msg->nheartbeats; i++)
    if (msg->heartbeats && msg->heartbeats[i])
      if (heartbeat_free (msg->heartbeats[i]))
        return 0;
  
  if (msg->idmefp)
    libidmef_list_delete (msg->idmefp);
  
  free (msg);
  
  return 1;
 }



