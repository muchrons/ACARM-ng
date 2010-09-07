/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2004 Sandro Poppi.  All rights reserved

Useful helper functions.

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: helpers.c,v 1.3 2009/02/15 14:15:04 alcapone Exp $
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <idmef_types.h>
#include <idmef_global.h>
#include <idmef_parser_helpers.h>

/* 0 = FALSE, 1 = TRUE */
int is_heartbeat (IDMEFmessage *msg)
 {
  if (!msg)
    return 0;
  
  if (msg->idmefp)
   {
    /* FIXME isn't there a more elegant way to do this? */
    /* 0 means equal */
    if (!strncmp ((char*)msg->idmefp->IDMEFXmlNodePtr->children->name, "Heartbeat", 9))
      return 1;
    else
      return 0;
   }
  else
    if (msg->nheartbeats)
      return 1;
    else
      return 0;
 }

/* 0 = FALSE, 1 = TRUE */
int is_alert (IDMEFmessage *msg)
 {
  if (!msg)
    return 0;
  
  if (msg->idmefp)
   {
    /* FIXME isn't there a more elegant way to do this? */
    /* 0 means equal */
    if (!strncmp ((char*)msg->idmefp->IDMEFXmlNodePtr->children->name, "Alert", 5))
      return 1;
    else
      return 0;
   }
  else
    if (msg->nalerts)
      return 1;
    else
      return 0;
 }

/*==========================================================================*
/* === added from idmefxml.c of libidmef-0.7.3                          === */
/*==========================================================================*

/*==========================================================================*
 * Name: timevalToNtpTimestamp
 *
 * Purpose: Creates a NTP timestamp with the provided timeval struct
 *
 * Returns: The provided NTP timestamp as a char *
 *
 * Example:
 *          struct timeval t;
 *          char *ntp_timestamp_string;
 *
 *          gettimeofday(&t,0);
 *          ntp_timestamp_string = timevalToNtpTimestamp(&t);
 *==========================================================================*/

xmlChar *timevalToNtpTimestamp(struct timeval *t)
{
   xmlChar *idmef_ntp_timestamp;
   unsigned long seconds, microseconds;

   seconds = t->tv_sec + 2208988800UL;
   microseconds = (4294967296UL * t->tv_usec) / 1000000UL;

   idmef_ntp_timestamp = (xmlChar *) xmlMalloc(sizeof(xmlChar) + MAX_NTP_TIMESTAMP_SIZE); /* 0xNNNNNNNN.0xNNNNNNNN */
   sprintf((char*)idmef_ntp_timestamp,"0x%lx.0x%lx", seconds, microseconds);

   return idmef_ntp_timestamp;
}

/*==========================================================================*
 * Name: timevalToDatetime *
 * Purpose: Formats the provided time as specified by the IDMEF XML
 *          . YYYY-MM-DDThh:mm:ssZ.  Time is represented as UTC.
 *
 * Returns: The formated time as a char *
 *
 * Example:
 *          time_t t;
 *          char *datetime_string;
 *
 *          t = time(NULL);
 *          datetime_string = timeToDatetime(&t);
 *
 * Note: This function outputs time in UTC, partially because we technically
 *       save space and bandwith, but more so because strftime's representation
 *       of "time-zone as hour offset from GMT", excludes the ':', which is
 *       required in the IDMEF spec.
 *==========================================================================*/

xmlChar *timevalToDatetime(struct timeval *t)
{
   xmlChar *date_str;
   struct tm *loc_time;

   loc_time = gmtime(&t->tv_sec);
   
   date_str = (xmlChar *) xmlMalloc(MAX_UTC_DATETIME_SIZE * sizeof(xmlChar));
   strftime((char*)date_str,MAX_UTC_DATETIME_SIZE,"%Y-%m-%dT%H:%M:%SZ",loc_time);
 
   return date_str;
}

/*==========================================================================*
 * Name: getStoredAlertID()
 *
 * Purpose: Opens the specified file, and attempts to extract the stored
 *          alert id.
 *
 * Returns: the alert id as an unsigned long integer, 1 if the file didn't
 *          contain an alert id, or 0 for error.
 *==========================================================================*/
unsigned long getStoredAlertID(char *f_name)
{
  FILE *f;
  char *idmef_alertid_string;
  unsigned long aid;

  /* Open the alert ID file and extract the stored alert ID if it exists*/
  f = fopen(f_name,"a+");
  if (!f) 
  {
     fprintf(stderr,"getStoredAlertID: IDMEF alert ID file open error (%s)\n",f_name);
     return 0;
  }

  /** Now seek back to the beginning... Nice little hack **/
  rewind(f);
    
  idmef_alertid_string = (char *) calloc(MAX_ALERTID_BUFFER_SIZE+1,sizeof(char));

  if(fgets(idmef_alertid_string, MAX_ALERTID_BUFFER_SIZE + 1, f) == NULL)
  {
    /*     fprintf(stderr,"getStoredAlertID: Stored alert ID not found in %s, continuing with alert ID = 1\n", f_name); */
     aid = 1;
  }
  else
     aid = strtoul (idmef_alertid_string, NULL, 10);

  free (idmef_alertid_string);
  fclose(f); /* done for now */
  
  return aid;
}

/*==========================================================================*
 * Name: saveAlertID()
 *
 * Purpose: Opens the specified file, and attempts to save the given
 *          alert id.
 *
 * Returns: 1 for success or 0 for error.
 *==========================================================================*/
int saveAlertID(unsigned long aid, char *f_name)
{
   char *temp;
   FILE *f;
 
   f = fopen(f_name,"w");
   
   if (!f) 
   {
      fprintf(stderr,"saveAlertID: IDMEF alert ID file open error (%s)\n",f_name);
      return 0;
   }

   temp = ulongToString(aid);

   if(fputs(temp, f) < 0)
   {
     fprintf(stderr,"saveAlertID: Error writting alert id (%lu) to %s\n", aid, f_name);
     free (temp);
     return 0;
   }
   
   free (temp);
   fputc('\n',f);
   fflush(f);
   fclose(f);

   return 1;
}

/*==========================================================================*
 * Name: intToString()
 * Purpose: Converts a signed integer value to it's ASCII representation.
 *
 * Parameters:
 *           - int_val : the value to be converted
 *
 * Returns: The ASCII representation. The returned string has to be free'd
 *          yourself!
 *==========================================================================*/
char * intToString(int int_val)
{
  int ret, tmp;
  char *string_val;
  
  if(int_val > 0)
    tmp = floor(log10(int_val)) + 2;
  else
     if(int_val < 0)
        tmp = floor(log10(-1 * int_val)) + 3;
     else
        tmp = 2;

  string_val = calloc(tmp,sizeof(char));
  
  if(string_val == NULL)
     return NULL;
  
  ret = snprintf(string_val,tmp,"%d",int_val);

  if(ret > -1 && ret <= tmp)
     return string_val;

  return NULL;
}

/*==========================================================================*
 * Name: ulongToString()
 * Purpose: Converts an unsigned value to it's ASCII representation.
 *
 * Parameters:
 *           - ulong_val : the value to be converted
 *
 * Returns: The ASCII representation The returned string has to be free'd
 *          yourself!
 *
 * Note: This function was custom made for converting ulong alertID's to
 *       strings.
 *==========================================================================*/
char * ulongToString(unsigned long ulong_val)
{
  int ret;
  unsigned long tmp;
  char *string_val;

  if(ulong_val == 0)
    tmp = 2;
  else
    tmp = floor(log10(ulong_val)) + 2;
          
  string_val = calloc (tmp,sizeof(char));
  
  if(string_val == NULL)
     return NULL;
  
  ret = snprintf(string_val,tmp,"%lu",ulong_val);

  if(ret > -1 && ret <= tmp)
     return string_val;

  return NULL;
}

