/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2004, 2005 Sandro Poppi.  All rights reserved

A SAX Parser for IDMEF messages

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: parser.c,v 1.5 2009/08/30 09:37:46 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ENABLE_NLS
#  include <libintl.h>
#else
#  define textdomain(String) (String)
#  define dgettext(Domain, String) (String)
#endif

#include <errno.h>
extern int errno;

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libxml/parser.h>

#include <idmef_types.h>
#include <idmef_global.h>
#include <idmef_strings.h>
#include <idmef_create.h>
#include <idmef_parser_helpers.h>
#include <idmef_list.h>

/* the states the parser could be in */
typedef enum
 {
  PARSER_START,						/* 0 */
  PARSER_IDMEF_MESSAGE,
  PARSER_ALERT,
  PARSER_ALERT_ANALYZER,
  PARSER_ALERT_ANALYZER_NODE,
  PARSER_ALERT_ANALYZER_NODE_NAME,
  PARSER_ALERT_ANALYZER_NODE_LOCATION,
  PARSER_ALERT_ANALYZER_NODE_ADDRESS,
  PARSER_ALERT_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_ALERT_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_ALERT_ANALYZER_PROCESS,
  PARSER_ALERT_ANALYZER_PROCESS_NAME,
  PARSER_ALERT_ANALYZER_PROCESS_PID,
  PARSER_ALERT_ANALYZER_PROCESS_PATH,
  PARSER_ALERT_ANALYZER_PROCESS_ARG,
  PARSER_ALERT_ANALYZER_PROCESS_ENV,
/* removed for recursive <Analyzer> elements
  PARSER_ALERT_ANALYZER_ANALYZER,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE_NAME,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE_LOCATION,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_NAME,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PID,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PATH,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ARG,
  PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ENV,
*/
  PARSER_ALERT_CREATETIME,
  PARSER_ALERT_ANALYZERTIME,
  PARSER_ALERT_DETECTTIME,
  PARSER_ALERT_SOURCE,
  PARSER_ALERT_SOURCE_NODE,
  PARSER_ALERT_SOURCE_NODE_NAME,
  PARSER_ALERT_SOURCE_NODE_LOCATION,
  PARSER_ALERT_SOURCE_NODE_ADDRESS,
  PARSER_ALERT_SOURCE_NODE_ADDRESS_NETMASK,
  PARSER_ALERT_SOURCE_NODE_ADDRESS_ADDRESS,
  PARSER_ALERT_SOURCE_PROCESS,
  PARSER_ALERT_SOURCE_PROCESS_NAME,
  PARSER_ALERT_SOURCE_PROCESS_PID,
  PARSER_ALERT_SOURCE_PROCESS_PATH,
  PARSER_ALERT_SOURCE_PROCESS_ARG,
  PARSER_ALERT_SOURCE_PROCESS_ENV,
  PARSER_ALERT_SOURCE_USER,
  PARSER_ALERT_SOURCE_USER_USERID,
  PARSER_ALERT_SOURCE_USER_USERID_NAME,
  PARSER_ALERT_SOURCE_USER_USERID_NUMBER,
  PARSER_ALERT_SOURCE_SERVICE,
  PARSER_ALERT_SOURCE_SERVICE_NAME,
  PARSER_ALERT_SOURCE_SERVICE_PORT,
  PARSER_ALERT_SOURCE_SERVICE_PORTLIST,
  PARSER_ALERT_SOURCE_SERVICE_PROTOCOL,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_OID,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE,
  PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_URL,
  PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_CGI,
  PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_ARG,
  PARSER_ALERT_TARGET,
  PARSER_ALERT_TARGET_NODE,
  PARSER_ALERT_TARGET_NODE_NAME,
  PARSER_ALERT_TARGET_NODE_LOCATION,
  PARSER_ALERT_TARGET_NODE_ADDRESS,
  PARSER_ALERT_TARGET_NODE_ADDRESS_NETMASK,
  PARSER_ALERT_TARGET_NODE_ADDRESS_ADDRESS,
  PARSER_ALERT_TARGET_PROCESS,
  PARSER_ALERT_TARGET_PROCESS_NAME,
  PARSER_ALERT_TARGET_PROCESS_PID,
  PARSER_ALERT_TARGET_PROCESS_PATH,
  PARSER_ALERT_TARGET_PROCESS_ARG,
  PARSER_ALERT_TARGET_PROCESS_ENV,
  PARSER_ALERT_TARGET_USER,
  PARSER_ALERT_TARGET_USER_USERID,
  PARSER_ALERT_TARGET_USER_USERID_NAME,
  PARSER_ALERT_TARGET_USER_USERID_NUMBER,
  PARSER_ALERT_TARGET_SERVICE,
  PARSER_ALERT_TARGET_SERVICE_NAME,
  PARSER_ALERT_TARGET_SERVICE_PORT,
  PARSER_ALERT_TARGET_SERVICE_PORTLIST,
  PARSER_ALERT_TARGET_SERVICE_PROTOCOL,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_OID,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ALERT_TARGET_SERVICE_WEBSERVICE,
  PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_URL,
  PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_CGI,
  PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_ARG,
  PARSER_ALERT_TARGET_FILE,
  PARSER_ALERT_TARGET_FILE_NAME,
  PARSER_ALERT_TARGET_FILE_PATH,
  PARSER_ALERT_TARGET_FILE_CREATE_TIME,
  PARSER_ALERT_TARGET_FILE_MODIFY_TIME,
  PARSER_ALERT_TARGET_FILE_ACCESS_TIME,
  PARSER_ALERT_TARGET_FILE_DATA_SIZE,
  PARSER_ALERT_TARGET_FILE_DISK_SIZE,
  PARSER_ALERT_TARGET_FILE_FILEACCESS,
  PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID,
  PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NAME,
  PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NUMBER,
  PARSER_ALERT_TARGET_FILE_FILEACCESS_PERMISSION,
  PARSER_ALERT_TARGET_FILE_LINKAGE,
  PARSER_ALERT_TARGET_FILE_LINKAGE_NAME,
  PARSER_ALERT_TARGET_FILE_LINKAGE_PATH,
  PARSER_ALERT_TARGET_FILE_INODE,
  PARSER_ALERT_TARGET_FILE_INODE_CHANGE_TIME,
  PARSER_ALERT_TARGET_FILE_INODE_NUMBER,
  PARSER_ALERT_TARGET_FILE_INODE_MAJOR_DEVICE,
  PARSER_ALERT_TARGET_FILE_INODE_MINOR_DEVICE,
  PARSER_ALERT_TARGET_FILE_INODE_C_MAJOR_DEVICE,
  PARSER_ALERT_TARGET_FILE_INODE_C_MINOR_DEVICE,
  PARSER_ALERT_TARGET_FILE_CHECKSUM,
  PARSER_ALERT_TARGET_FILE_CHECKSUM_VALUE,
  PARSER_ALERT_TARGET_FILE_CHECKSUM_KEY,
  PARSER_ALERT_CLASSIFICATION,
  PARSER_ALERT_CLASSIFICATION_REFERENCE,
  PARSER_ALERT_CLASSIFICATION_REFERENCE_NAME,
  PARSER_ALERT_CLASSIFICATION_REFERENCE_URL,
  PARSER_ALERT_ASSESSMENT,
  PARSER_ALERT_ASSESSMENT_IMPACT,
  PARSER_ALERT_ASSESSMENT_ACTION,
  PARSER_ALERT_ASSESSMENT_CONFIDENCE,
  PARSER_ALERT_ADDITIONALDATA,
  PARSER_ALERT_ADDITIONALDATA_BOOLEAN,
  PARSER_ALERT_ADDITIONALDATA_BYTE,
  PARSER_ALERT_ADDITIONALDATA_CHARACTER,
  PARSER_ALERT_ADDITIONALDATA_DATE_TIME,
  PARSER_ALERT_ADDITIONALDATA_INTEGER,
  PARSER_ALERT_ADDITIONALDATA_NTPSTAMP,
  PARSER_ALERT_ADDITIONALDATA_PORTLIST,
  PARSER_ALERT_ADDITIONALDATA_REAL,
  PARSER_ALERT_ADDITIONALDATA_STRING,
  PARSER_ALERT_ADDITIONALDATA_BYTE_STRING,
  PARSER_ALERT_ADDITIONALDATA_XMLTEXT,
  PARSER_ALERT_TOOLALERT,
  PARSER_ALERT_TOOLALERT_NAME,
  PARSER_ALERT_TOOLALERT_COMMAND,
  PARSER_ALERT_TOOLALERT_ALERTIDENT,
  PARSER_ALERT_OVERFLOWALERT,
  PARSER_ALERT_OVERFLOWALERT_PROGRAM,
  PARSER_ALERT_OVERFLOWALERT_SIZE,
  PARSER_ALERT_OVERFLOWALERT_BUFFER,
  PARSER_ALERT_CORRELATIONALERT,
  PARSER_ALERT_CORRELATIONALERT_NAME,
  PARSER_ALERT_CORRELATIONALERT_ALERTIDENT,
  PARSER_HEARTBEAT,
  PARSER_HEARTBEAT_ANALYZER,
  PARSER_HEARTBEAT_ANALYZER_NODE,
  PARSER_HEARTBEAT_ANALYZER_NODE_NAME,
  PARSER_HEARTBEAT_ANALYZER_NODE_LOCATION,
  PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS,
  PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_HEARTBEAT_ANALYZER_PROCESS,
  PARSER_HEARTBEAT_ANALYZER_PROCESS_NAME,
  PARSER_HEARTBEAT_ANALYZER_PROCESS_PID,
  PARSER_HEARTBEAT_ANALYZER_PROCESS_PATH,
  PARSER_HEARTBEAT_ANALYZER_PROCESS_ARG,
  PARSER_HEARTBEAT_ANALYZER_PROCESS_ENV,
/*  PARSER_HEARTBEAT_ANALYZER_ANALYZER,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_NAME,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_LOCATION,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_NAME,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PID,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PATH,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ARG,
  PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ENV,
*/
  PARSER_HEARTBEAT_CREATETIME,
  PARSER_HEARTBEAT_ANALYZERTIME,
  PARSER_HEARTBEAT_HEARTBEATINTERVAL, /* draft 13 */
  PARSER_HEARTBEAT_ADDITIONALDATA,
  PARSER_HEARTBEAT_ADDITIONALDATA_BOOLEAN,
  PARSER_HEARTBEAT_ADDITIONALDATA_BYTE,
  PARSER_HEARTBEAT_ADDITIONALDATA_CHARACTER,
  PARSER_HEARTBEAT_ADDITIONALDATA_DATE_TIME,
  PARSER_HEARTBEAT_ADDITIONALDATA_INTEGER,
  PARSER_HEARTBEAT_ADDITIONALDATA_NTPSTAMP,
  PARSER_HEARTBEAT_ADDITIONALDATA_PORTLIST,
  PARSER_HEARTBEAT_ADDITIONALDATA_REAL,
  PARSER_HEARTBEAT_ADDITIONALDATA_STRING,
  PARSER_HEARTBEAT_ADDITIONALDATA_BYTE_STRING,
  PARSER_HEARTBEAT_ADDITIONALDATA_XMLTEXT,
#ifdef IDMEF_ASSET
  PARSER_ASSET,
  PARSER_ASSET_ANALYZER,
  PARSER_ASSET_ANALYZER_NODE,
  PARSER_ASSET_ANALYZER_NODE_NAME,
  PARSER_ASSET_ANALYZER_NODE_LOCATION,
  PARSER_ASSET_ANALYZER_NODE_ADDRESS,
  PARSER_ASSET_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_ANALYZER_PROCESS,
  PARSER_ASSET_ANALYZER_PROCESS_NAME,
  PARSER_ASSET_ANALYZER_PROCESS_PID,
  PARSER_ASSET_ANALYZER_PROCESS_PATH,
  PARSER_ASSET_ANALYZER_PROCESS_ARG,
  PARSER_ASSET_ANALYZER_PROCESS_ENV,
/*  PARSER_ASSET_ANALYZER_ANALYZER,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE_NAME,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE_LOCATION,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_NAME,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PID,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PATH,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ARG,
  PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ENV,
*/
  PARSER_ASSET_TARGET,
  PARSER_ASSET_TARGET_NODE,
  PARSER_ASSET_TARGET_NODE_NAME,
  PARSER_ASSET_TARGET_NODE_LOCATION,
  PARSER_ASSET_TARGET_NODE_ADDRESS,
  PARSER_ASSET_TARGET_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TARGET_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TARGET_PROCESS,
  PARSER_ASSET_TARGET_PROCESS_NAME,
  PARSER_ASSET_TARGET_PROCESS_PID,
  PARSER_ASSET_TARGET_PROCESS_PATH,
  PARSER_ASSET_TARGET_PROCESS_ARG,
  PARSER_ASSET_TARGET_PROCESS_ENV,
  PARSER_ASSET_TARGET_USER,
  PARSER_ASSET_TARGET_USER_USERID,
  PARSER_ASSET_TARGET_USER_USERID_NAME,
  PARSER_ASSET_TARGET_USER_USERID_NUMBER,
  PARSER_ASSET_TARGET_SERVICE,
  PARSER_ASSET_TARGET_SERVICE_NAME,
  PARSER_ASSET_TARGET_SERVICE_PORT,
  PARSER_ASSET_TARGET_SERVICE_PORTLIST,
  PARSER_ASSET_TARGET_SERVICE_PROTOCOL,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_OID,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ASSET_TARGET_SERVICE_WEBSERVICE,
  PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_URL,
  PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_CGI,
  PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_ARG,
  PARSER_ASSET_TARGET_FILE,
  PARSER_ASSET_TARGET_FILE_NAME,
  PARSER_ASSET_TARGET_FILE_PATH,
  PARSER_ASSET_TARGET_FILE_CREATE_TIME,
  PARSER_ASSET_TARGET_FILE_MODIFY_TIME,
  PARSER_ASSET_TARGET_FILE_ACCESS_TIME,
  PARSER_ASSET_TARGET_FILE_DATA_SIZE,
  PARSER_ASSET_TARGET_FILE_DISK_SIZE,
  PARSER_ASSET_TARGET_FILE_FILEACCESS,
  PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID,
  PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NAME,
  PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NUMBER,
  PARSER_ASSET_TARGET_FILE_FILEACCESS_PERMISSION,
  PARSER_ASSET_TARGET_FILE_LINKAGE,
  PARSER_ASSET_TARGET_FILE_LINKAGE_NAME,
  PARSER_ASSET_TARGET_FILE_LINKAGE_PATH,
  PARSER_ASSET_TARGET_FILE_INODE,
  PARSER_ASSET_TARGET_FILE_INODE_CHANGE_TIME,
  PARSER_ASSET_TARGET_FILE_INODE_NUMBER,
  PARSER_ASSET_TARGET_FILE_INODE_MAJOR_DEVICE,
  PARSER_ASSET_TARGET_FILE_INODE_MINOR_DEVICE,
  PARSER_ASSET_TARGET_FILE_INODE_C_MAJOR_DEVICE,
  PARSER_ASSET_TARGET_FILE_INODE_C_MINOR_DEVICE,
  PARSER_ASSET_TARGET_FILE_CHECKSUM,
  PARSER_ASSET_TARGET_FILE_CHECKSUM_VALUE,
  PARSER_ASSET_TARGET_FILE_CHECKSUM_KEY,
  PARSER_ASSET_TOOL,
  PARSER_ASSET_TOOL_URL,
  PARSER_ASSET_TOOL_ARG,
  PARSER_ASSET_TOOL_ADDITIONALDATA,
  PARSER_ASSET_TOOL_ADDITIONALDATA_BOOLEAN,
  PARSER_ASSET_TOOL_ADDITIONALDATA_BYTE,
  PARSER_ASSET_TOOL_ADDITIONALDATA_CHARACTER,
  PARSER_ASSET_TOOL_ADDITIONALDATA_DATE_TIME,
  PARSER_ASSET_TOOL_ADDITIONALDATA_INTEGER,
  PARSER_ASSET_TOOL_ADDITIONALDATA_NTPSTAMP,
  PARSER_ASSET_TOOL_ADDITIONALDATA_PORTLIST,
  PARSER_ASSET_TOOL_ADDITIONALDATA_REAL,
  PARSER_ASSET_TOOL_ADDITIONALDATA_STRING,
  PARSER_ASSET_TOOL_ADDITIONALDATA_BYTE_STRING,
  PARSER_ASSET_TOOL_ADDITIONALDATA_XMLTEXT,
  PARSER_ASSET_TOOL_RESULT,
  PARSER_ASSET_TOOL_RESULT_CONFIDENCE,
  PARSER_ASSET_TOOL_RESULT_SERVICE,
  PARSER_ASSET_TOOL_RESULT_SERVICE_NAME,
  PARSER_ASSET_TOOL_RESULT_SERVICE_PORT,
  PARSER_ASSET_TOOL_RESULT_SERVICE_PORTLIST,
  PARSER_ASSET_TOOL_RESULT_SERVICE_PROTOCOL,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_OID,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE,
  PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_URL,
  PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_CGI,
  PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_ARG,
  PARSER_ASSET_TOOL_RESULT_VULNERABILITY,
  PARSER_ASSET_TOOL_RESULT_VULNERABILITY_EXPLOIT,
  PARSER_ASSET_TOOL_RESULT_OS,
  PARSER_ASSET_TOOL_RESULT_OS_PATCHLEVEL,
  PARSER_ASSET_TOOL_RESULT_OS_HOTFIX,
  PARSER_ASSET_TOOL_RESULT_RULE,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE_LOCATION,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_USER,
  PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID,
  PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID_NUMBER,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PORT,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PORTLIST,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PROTOCOL,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_OID,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_URL,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_CGI,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_ARG,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_LOCATION,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_LOCATION,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PORT,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PORTLIST,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PROTOCOL,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_OID,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_URL,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_CGI,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_ARG,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_LOCATION,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_LOCATION,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS_NETMASK,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS_ADDRESS,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_NAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PORT,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PORTLIST,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PROTOCOL,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_OID,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_COMMUNITY,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_SECURITYNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_CONTEXTNAME,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_CONTEXTENGINEID,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_COMMAND,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_URL,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_CGI,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_HTTP_METHOD,
  PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_ARG,
  PARSER_ASSET_TOOL_RESULT_RULE_FWTYPE,
  PARSER_ASSET_TOOL_RESULT_RULE_FWVERSION,
  PARSER_ASSET_TOOL_RESULT_REFERENCE,
  PARSER_ASSET_TOOL_RESULT_REFERENCE_NAME,
  PARSER_ASSET_TOOL_RESULT_REFERENCE_URL,
  PARSER_ASSET_TOOL_RESULT_CREATETIME,
  PARSER_ASSET_TOOL_RESULT_BANNER,
  PARSER_ASSET_TOOL_RESULT_SUMMARY,
  PARSER_ASSET_TOOL_RESULT_DESCRIPTION,
#endif
  PARSER_UNKNOWN,
  PARSER_END
 } ParserState;

typedef struct _SAXParserState
 {
  ParserState state;
  ParserState prevstate;
  unsigned int unknown_depth; /* handle recursive unrecognised tags */
  const char *domain;
  char *content;
  
  IDMEFmessage *msg;
//  IDMEFPtr idmefp;   /* the libxml version of the message */

  unsigned int gen_idmefp; /* 0: don't generate IDMEFPtr elements, the default
  			      1: generate IDMEFPtr elements */
  int analyzer_count; /* counter for analyzer path tracking */
 } SAXParserState;


static xmlEntityPtr idmef_parser_get_entity (SAXParserState *state, const xmlChar *name);
static int idmef_parser_start_document (SAXParserState *state);
static void idmef_parser_end_document (SAXParserState *state);
static int idmef_parser_start_element (SAXParserState *state,
			                const xmlChar *name, const xmlChar **attrs);
static void idmef_parser_end_element (SAXParserState *state, const xmlChar *name);
static void idmef_parser_characters (SAXParserState *state, const xmlChar *chars, int len);
static void idmef_parser_warning (SAXParserState *state, const char *msg, ...);
static void idmef_parser_error (SAXParserState *state, const char *msg, ...);
static int idmef_parser_fatal_error (SAXParserState *state, int errn, const char *msg, ...);


static xmlSAXHandler IDMEFHandler =
 {
  (internalSubsetSAXFunc)NULL,                       /* internalSubset */
  (isStandaloneSAXFunc)NULL,                         /* isStandalone */
  (hasInternalSubsetSAXFunc)NULL,                    /* hasInternalSubset */
  (hasExternalSubsetSAXFunc)NULL,                    /* hasExternalSubset */
  (resolveEntitySAXFunc)NULL,                        /* resolveEntity */
  (getEntitySAXFunc)idmef_parser_get_entity,           /* getEntity */
  (entityDeclSAXFunc)NULL,                           /* entityDecl */
  (notationDeclSAXFunc)NULL,                         /* notationDecl */
  (attributeDeclSAXFunc)NULL,                        /* attributeDecl */
  (elementDeclSAXFunc)NULL,                          /* elementDecl */
  (unparsedEntityDeclSAXFunc)NULL,                   /* unparsedEntityDecl */
  (setDocumentLocatorSAXFunc)NULL,                   /* setDocumentLocator */
  (startDocumentSAXFunc)idmef_parser_start_document,   /* startDocument */
  (endDocumentSAXFunc)idmef_parser_end_document,       /* endDocument */
  (startElementSAXFunc)idmef_parser_start_element,     /* startElement */
  (endElementSAXFunc)idmef_parser_end_element,         /* endElement */
  (referenceSAXFunc)NULL,                            /* reference */
  (charactersSAXFunc)idmef_parser_characters,          /* characters */
  (ignorableWhitespaceSAXFunc)NULL,                  /* ignorableWhitespace */
  (processingInstructionSAXFunc)NULL,                /* processingInstruction */
  (commentSAXFunc)NULL,                              /* comment */
  (warningSAXFunc)idmef_parser_warning,                /* warning */
  (errorSAXFunc)idmef_parser_error,                    /* error */
  (fatalErrorSAXFunc)idmef_parser_fatal_error,         /* fatalError */
 };


/* helper function for analyzer path tracking */
/* TODO It seems we're only using count == -1 so we could simplify (and
   inline?) this function */
static IDMEFanalyzer* get_alert_analyzer (SAXParserState *state, int count)
 {
  IDMEFanalyzer *analyzer;
  int i;
  
#ifdef IDMEF_DEBUG
  fprintf (stderr, "get_alert_analyzer called with count = %d\n", count);
#endif

  if (!state || !state->msg || !state->msg->alerts[state->msg->nalerts-1])
    return NULL;
    
  /* walk through the analyzers to find the correct one */
  analyzer = state->msg->alerts[state->msg->nalerts-1]->analyzer;        

  if (count == -1)
   {
    /* count == -1 means: find the last analyzer */

#ifdef IDMEF_DEBUG
    fprintf (stderr, "get_alert_analyzer: state->analyzer_count = %d\n", state->analyzer_count); 
#endif
    
    while (analyzer->analyzer)
     {
      analyzer = analyzer->analyzer;
     }
   }
  else /* this is reached for closing </Analyzer> tags */
    for (i = 1; i <= count; i++)
     {

#ifdef IDMEF_DEBUG
      fprintf (stderr, "get_alert_analyzer: In for loop: count = %d, i = %d\n", count, i);
#endif

      if (count > 1 && analyzer->analyzer)
        analyzer = analyzer->analyzer;
      else
       {
        idmef_parser_warning (state, "While searching the analyzer #%d we "
                            "reached the end of the analyzer list. Check your "
                            "program!");
       }
     }

  return analyzer;
 }
  
/* helper function for analyzer path tracking */
/* TODO It seems we're only using count == -1 so we could simplify (and
   inline?) this function */
static IDMEFanalyzer* get_heartbeat_analyzer (SAXParserState *state, int count)
 {
  IDMEFanalyzer *analyzer;
  int i;
  
  if (!state || !state->msg || !state->msg->heartbeats[state->msg->nheartbeats-1])
    return NULL;
    
  /* walk through the analyzers to find the correct one */
  analyzer = state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer;        

  if (count == -1)
   {
    /* count == -1 means: find the last analyzer */

#ifdef IDMEF_DEBUG
    fprintf (stderr, "get_heartbeat_analyzer: state->analyzer_count = %d\n", state->analyzer_count); 
#endif
    
    while (analyzer->analyzer)
     {
      analyzer = analyzer->analyzer;
     }
   }
  else /* this is reached for closing </Analyzer> tags */
    for (i = 1; i <= count; i++)
     {

#ifdef IDMEF_DEBUG
      fprintf (stderr, "get_heartbeat_analyzer: In for loop: count = %d, i = %d\n", count, i);
#endif

      if (count > 1 && analyzer->analyzer)
        analyzer = analyzer->analyzer;
      else
       {
        idmef_parser_warning (state, "While searching the analyzer #%d we "
                            "reached the end of the analyzer list. Check your "
                            "program!");
       }
     }

  return analyzer;
 }
  
#ifdef IDMEF_ASSET
/* helper function for analyzer path tracking */
/* TODO It seems we're only using count == -1 so we could simplify (and
   inline?) this function */
static IDMEFanalyzer* get_asset_analyzer (SAXParserState *state, int count)
 {
  IDMEFanalyzer *analyzer;
  int i;
  
  if (!state || !state->msg || !state->msg->assets[state->msg->nassets-1])
    return NULL;
    
  /* walk through the analyzers to find the correct one */
  analyzer = state->msg->assets[state->msg->nassets-1]->analyzer;        

  if (count == -1)
   {
    /* count == -1 means: find the last analyzer */

#ifdef IDMEF_DEBUG
    fprintf (stderr, "get_asset_analyzer: state->analyzer_count = %d\n", state->analyzer_count); 
#endif
    
    while (analyzer->analyzer)
     {
      analyzer = analyzer->analyzer;
     }
   }
  else /* this is reached for closing </Analyzer> tags */
    for (i = 1; i <= count; i++)
     {

#ifdef IDMEF_DEBUG
      fprintf (stderr, "get_asset_analyzer: In for loop: count = %d, i = %d\n", count, i);
#endif

      if (count > 1 && analyzer->analyzer)
        analyzer = analyzer->analyzer;
      else
       {
        idmef_parser_warning (state, "While searching the analyzer #%d we "
                            "reached the end of the analyzer list. Check your "
                            "program!");
       }
     }

  return analyzer;
 }
#endif
  
static xmlEntityPtr idmef_parser_get_entity (SAXParserState *state, const xmlChar *name)
 {
  return xmlGetPredefinedEntity (name);
 }

static int idmef_parser_start_document (SAXParserState *state)
 {
  state->state     = PARSER_START;
  state->prevstate = PARSER_UNKNOWN;
  state->unknown_depth = 0;
  state->analyzer_count = 0;

  if (!(state->content = (char*)calloc(sizeof(char), 128)))
    return idmef_parser_fatal_error (NULL, ENOMEM, "Memory exhausted!");
  
//  state->idmefp    = NULL;

#ifdef IDMEF_DEBUG
  fprintf (stdout, "PARSER_END = %d\n", PARSER_END);
#endif
 }

static void idmef_parser_end_document (SAXParserState *state)
 {
  if (state->content)
    free (state->content);

  if (state->unknown_depth != 0)
    idmef_parser_warning(state, "unknown_depth != 0 (%d)", state->unknown_depth);
 }
 
static int idmef_parser_start_element (SAXParserState *state,
			              const xmlChar *name, const xmlChar **attrs)
 {
  int i;
  IDMEFPtr tidmefp = NULL; /* temporary pointer */
  void *ptr; /* temp. pointer for handling i_renew errors */
  
#ifdef IDMEF_DEBUG
  fprintf (stdout, "idmef_parser_start_element called for <%s>, state %d, "
  	     "prevstate = %d\n", name, state->state, state->prevstate);
#endif

  switch (state->state)
   {
    case PARSER_START:
    	if (!strcmp ((char*)name, "IDMEF-Message"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state IDMEF-Message\n");
#endif
          state->state = PARSER_IDMEF_MESSAGE;
          state->prevstate = PARSER_START;
          
          /* create new message object */
          state->msg = i_new0 (IDMEFmessage, 1);
          
          if (!state->msg)
            return idmef_parser_fatal_error (NULL, ENOMEM,
                    "idmef-parser: Cannot allocate memory (msg)! Exiting ...");
          
          state->msg->attlist = i_new0 (IDMEFattlist_global, 1);
          
          if (!state->msg->attlist)
            return idmef_parser_fatal_error (NULL, ENOMEM,
                     "idmef-parser: Cannot allocate memory (attlist)! Exiting ...");
          
	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp ((char*)attrs[i], "version") && !strcmp ((char*)attrs[i+1], "1.0"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "IDMEF-Message version correct\n");
#endif
	      state->msg->version = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!state->msg->idmefp)
                  state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp = newIDMEF_Message_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (state->msg->idmefp, tidmefp);
                 }
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (((!strcmp ((char*)attrs[i], "xmlns")) &&
                      (!strcmp ((char*)attrs[i+1], "urn:iana:xml:ns:idmef"))))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "%s = %s\n", attrs[i], attrs[i+1]);
#endif
              state->msg->attlist->xmlns = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!state->msg->idmefp)
                  state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_xmlns ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newIDMEF_Message_xmlns ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (state->msg->idmefp, tidmefp);
                 }
              
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (((!strcmp ((char*)attrs[i], "xmlns:idmef")) &&
                      (!strcmp ((char*)attrs[i+1], "urn:iana:xml:ns:idmef"))))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "%s = %s\n", attrs[i], attrs[i+1]);
#endif
              state->msg->attlist->xmlns_idmef = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!state->msg->idmefp)
                  state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_xmlns_idmef ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newIDMEF_Message_xmlns_idmef ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (state->msg->idmefp, tidmefp);
                 }
              
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp ((char*)attrs[i], "xml:lang"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "%s = %s\n", attrs[i], attrs[i+1]);
#endif
              state->msg->attlist->xml_lang = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!state->msg->idmefp)
                  state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_xml_lang ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newIDMEF_Message_xml_lang ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (state->msg->idmefp, tidmefp);
                 }
              
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (((!strcmp ((char*)attrs[i], "xml:space")) &&
                     ((!(strcmp ((char*)attrs[i+1], "default")) ||
                       !(strcmp ((char*)attrs[i+1], "preserve"))))))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "%s = %s\n", attrs[i], attrs[i+1]);
#endif
              state->msg->attlist->xml_space = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!state->msg->idmefp)
                  state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_xml_space ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newIDMEF_Message_xml_space ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (state->msg->idmefp, tidmefp);
                 }
              
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <IDMEF-Message>",
                        attrs[i], attrs[i+1]);
                        
            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
	   } /* for */
           
           /* create an idmef object even if the element is without attributes:
              <IDMEF-Message> */
           if (state->gen_idmefp && !state->msg->idmefp)
            {
             state->msg->idmefp = newIDMEF_Message (
              	                newIDMEF_Message_version (IDMEF_MESSAGE_VERSION, NULL));
            }
         }
        else
         {
          idmef_parser_warning (state, "Expected <IDMEF-Message>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
        
    case PARSER_IDMEF_MESSAGE:
    	if (!strcmp ((char*)name, "Alert"))
         {
          IDMEFmessage *msg = state->msg;
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert\n");
#endif
          state->state = PARSER_ALERT;
          state->prevstate = PARSER_IDMEF_MESSAGE;
          msg->nalerts++;
          ptr = i_renew (IDMEFalert*, msg->alerts,
          			 msg->nalerts);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          msg->alerts = ptr;
          msg->alerts[msg->nalerts-1] = i_new0 (IDMEFalert, 1);
	  
          if (!msg->alerts[msg->nalerts-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    msg->alerts[msg->nalerts-1]->idmefp =
	        newAlert (NULL);
                
            /* combine the alert with the message node */
            libidmef_list_add_element (msg->idmefp, msg->alerts[msg->nalerts-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "messageid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "Alert messageid = %s\n", attrs[i+1]);
#endif
	      msg->alerts[msg->nalerts-1]->messageid = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!msg->alerts[msg->nalerts-1]->idmefp)
                  msg->alerts[msg->nalerts-1]->idmefp =
                    newAlert (newAlert_messageid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAlert_messageid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (msg->alerts[msg->nalerts-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (msg->idmefp, msg->alerts[msg->nalerts-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Alert>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
	   } /* for */
         }
        else if (!strcmp ((char*)name, "Heartbeat"))
         {
          IDMEFmessage *msg = state->msg;

#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat\n");
#endif
          state->state = PARSER_HEARTBEAT;
          state->prevstate = PARSER_IDMEF_MESSAGE;
          msg->nheartbeats++;
          ptr = i_renew (IDMEFheartbeat*, msg->heartbeats,
          			     msg->nheartbeats);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          msg->heartbeats = ptr;
          msg->heartbeats[msg->nheartbeats-1] = i_new0 (IDMEFheartbeat, 1);

          if (!msg->heartbeats[msg->nheartbeats-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    msg->heartbeats[msg->nheartbeats-1]->idmefp =
	        newHeartbeat (NULL);
            /* combine the heartbeat with the message node */
            libidmef_list_add_element (msg->idmefp, msg->heartbeats[msg->nheartbeats-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "messageid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "Heartbeat messageid = %s\n", attrs[i+1]);
#endif
	      msg->heartbeats[msg->nheartbeats-1]->messageid = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!msg->heartbeats[msg->nheartbeats-1]->idmefp)
                  msg->heartbeats[msg->nheartbeats-1]->idmefp =
                    newHeartbeat (newHeartbeat_messageid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newHeartbeat_messageid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (msg->heartbeats[msg->nheartbeats-1]->idmefp, tidmefp);
                 }

                /* combine the heartbeat with the message node */
                libidmef_list_add_element (msg->idmefp, msg->heartbeats[msg->nheartbeats-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Heartbeat>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
	   } /* for */
         }
#ifdef IDMEF_ASSET
    	else if (!strcmp ((char*)name, "Asset"))
         {
          IDMEFmessage *msg = state->msg;
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset\n");
#endif
          state->state = PARSER_ASSET;
          state->prevstate = PARSER_IDMEF_MESSAGE;
          msg->nassets++;
          ptr = i_renew (IDMEFasset*, msg->assets,
          			 msg->nassets);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          msg->assets = ptr;
          msg->assets[msg->nassets-1] = i_new0 (IDMEFasset, 1);
	  
          if (!msg->assets[msg->nassets-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    msg->assets[msg->nassets-1]->idmefp =
	        newAsset (NULL);
                
            /* combine the asset with the message node */
            libidmef_list_add_element (msg->idmefp, msg->assets[msg->nassets-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "Asset ident = %s\n", attrs[i+1]);
#endif
	      msg->assets[msg->nassets-1]->ident = strdup ((char*)attrs[i+1]);
              
              if (state->gen_idmefp)
               {
                if (!msg->assets[msg->nassets-1]->idmefp)
                  msg->assets[msg->nassets-1]->idmefp =
                    newAsset (newAsset_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAsset_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (msg->assets[msg->nassets-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (msg->idmefp, msg->assets[msg->nassets-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Asset>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
	   } /* for */
         }

#endif /* IDMEF_ASSET */
        else
         {
#ifdef IDMEF_ASSET
          idmef_parser_warning (state, "Expected <Alert|Heartbeat|Asset>, got <%s>.", name);
#else
          idmef_parser_warning (state, "Expected <Alert|Heartbeat>, got <%s>.", name);
#endif
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
        
    case PARSER_ALERT:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        
    	if (!strcmp ((char*)name, "Analyzer"))
         {
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer\n");
#endif
          state->state = PARSER_ALERT_ANALYZER;
          state->prevstate = PARSER_ALERT;
          state->analyzer_count++;
          alert->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!alert->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the analyzer with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              alert->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              alert->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              alert->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              alert->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              alert->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              alert->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              alert->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              alert->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzer->idmefp)
                  alert->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
           } /* for */ 
         }
        else if (!strcmp ((char*)name, "CreateTime"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert CreateTime\n");
#endif
          state->state = PARSER_ALERT_CREATETIME;
          state->prevstate = PARSER_ALERT;
          alert->createtime = i_new0 (IDMEFtime, 1);

          if (!alert->createtime)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->createtime->idmefp = newCreateTime (NULL);

            /* combine the createtime with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->createtime->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ntpstamp"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ntpstamp = %s\n", attrs[i+1]);
#endif
              alert->createtime->ntpstamp = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->createtime->idmefp)
                  alert->createtime->idmefp =
                    newCreateTime ((char*)"", newCreateTime_ntpstamp ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newCreateTime_ntpstamp ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->createtime->idmefp, tidmefp);
                 }

                /* combine the createtime with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->createtime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <CreateTime>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Classification"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Classification\n");
#endif
          state->state = PARSER_ALERT_CLASSIFICATION;
          state->prevstate = PARSER_ALERT;
          alert->classification = i_new0 (IDMEFclassification, 1);

          if (!alert->classification)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->classification->idmefp =
	        newClassification (NULL);

            /* combine the classification with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->classification->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      alert->classification->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->classification->idmefp)
                  alert->classification->idmefp =
                    newClassification (newClassification_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newClassification_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->classification->idmefp, tidmefp);
                 }
 
                /* combine the classification with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->classification->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "text"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "text = %s\n", attrs[i+1]);
#endif
	      alert->classification->text = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->classification->idmefp)
                  alert->classification->idmefp =
                    newClassification (newClassification_text ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newClassification_text ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->classification->idmefp, tidmefp);
                 }

                /* combine the classification with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->classification->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Classification>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "DetectTime"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert DetectTime\n");
#endif
          state->state = PARSER_ALERT_DETECTTIME;
          state->prevstate = PARSER_ALERT;
          alert->detecttime = i_new0 (IDMEFtime, 1);

          if (!alert->detecttime)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->detecttime->idmefp =
	        newDetectTime (NULL);

            /* combine the detecttime with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->detecttime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ntpstamp"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ntpstamp = %s\n", attrs[i+1]);
#endif
              alert->detecttime->ntpstamp = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->detecttime->idmefp)
                  alert->detecttime->idmefp =
                    newDetectTime ((char*)"", newDetectTime_ntpstamp ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newDetectTime_ntpstamp ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->detecttime->idmefp, tidmefp);
                 }

                /* combine the detecttime with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->detecttime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <DetectTime>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "AnalyzerTime"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert AnalyzerTime\n");
#endif
          state->state = PARSER_ALERT_ANALYZERTIME;
          state->prevstate = PARSER_ALERT;
          alert->analyzertime = i_new0 (IDMEFtime, 1);

          if (!alert->analyzertime)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->analyzertime->idmefp =
	        newAnalyzerTime (NULL);

            /* combine the analyzertime with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->analyzertime->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ntpstamp"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ntpstamp = %s\n", attrs[i+1]);
#endif
              alert->analyzertime->ntpstamp = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->analyzertime->idmefp)
                  alert->analyzertime->idmefp =
                    newAnalyzerTime ((char*)"", newAnalyzerTime_ntpstamp ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzerTime_ntpstamp ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->analyzertime->idmefp, tidmefp);
                 }

                /* combine the detecttime with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->analyzertime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <AnalyzerTime>",
                        attrs[i], attrs[i+1]);
               }
	   }
         }
        else if (!strcmp ((char*)name, "Source"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source\n");
#endif
          state->state = PARSER_ALERT_SOURCE;
          state->prevstate = PARSER_ALERT;
          alert->nsources++;
          ptr = i_renew (IDMEFsource*, alert->sources,
          			    alert->nsources);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          alert->sources = ptr;
          alert->sources[alert->nsources-1] = i_new0 (IDMEFsource, 1);

          if (!alert->sources[alert->nsources-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->sources[alert->nsources-1]->idmefp =
	        newSource (NULL);

            /* combine the source with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->sources[alert->nsources-1]->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
          
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      alert->sources[alert->nsources-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->sources[alert->nsources-1]->idmefp)
                  alert->sources[alert->nsources-1]->idmefp =
                    newSource (newSource_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newSource_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->sources[alert->nsources-1]->idmefp, tidmefp);
                 }

                /* combine the source with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->sources[alert->nsources-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "spoofed"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "spoofed = %s\n", attrs[i+1]);
#endif
	      alert->sources[alert->nsources-1]->spoofed =
                 get_spoofed ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->sources[alert->nsources-1]->idmefp)
                  alert->sources[alert->nsources-1]->idmefp =
                    newSource (newSource_spoofed ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newSource_spoofed ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->sources[alert->nsources-1]->idmefp, tidmefp);
                 }

                /* combine the source with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->sources[alert->nsources-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "interface"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "interface = %s\n", attrs[i+1]);
#endif
	      alert->sources[alert->nsources-1]->interface = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->sources[alert->nsources-1]->idmefp)
                  alert->sources[alert->nsources-1]->idmefp =
                    newSource (newSource_interface ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newSource_interface ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->sources[alert->nsources-1]->idmefp, tidmefp);
                 }

                /* combine the source with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->sources[alert->nsources-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Source>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Target"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target\n");
#endif
          state->state = PARSER_ALERT_TARGET;
          state->prevstate = PARSER_ALERT;
          alert->ntargets++;
          ptr = i_renew (IDMEFtarget*, alert->targets,
          			    alert->ntargets);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          alert->targets = ptr;
	  alert->targets[alert->ntargets-1] = i_new0 (IDMEFtarget, 1);

          if (!alert->targets[alert->ntargets-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    alert->targets[alert->ntargets-1]->idmefp =
	        newTarget (NULL);

            /* combine the target with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->targets[alert->ntargets-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
          
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      alert->targets[alert->ntargets-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->targets[alert->ntargets-1]->idmefp)
                  alert->targets[alert->ntargets-1]->idmefp =
                    newTarget (newTarget_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->targets[alert->ntargets-1]->idmefp, tidmefp);
                 }

                /* combine the target with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->targets[alert->ntargets-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "decoy"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "decoy = %s\n", attrs[i+1]);
#endif
	      alert->targets[alert->ntargets-1]->decoy =
                get_decoy ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->targets[alert->ntargets-1]->idmefp)
                  alert->targets[alert->ntargets-1]->idmefp =
                    newTarget (newTarget_decoy ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_decoy ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->targets[alert->ntargets-1]->idmefp, tidmefp);
                 }

                /* combine the target with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->targets[alert->ntargets-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "interface"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "interface = %s\n", attrs[i+1]);
#endif
	      alert->targets[alert->ntargets-1]->interface = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->targets[alert->ntargets-1]->idmefp)
                  alert->targets[alert->ntargets-1]->idmefp =
                    newTarget (newTarget_interface ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_interface ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->targets[alert->ntargets-1]->idmefp, tidmefp);
                 }

                /* combine the target with the alert node */
                libidmef_list_add_element (alert->idmefp, alert->targets[alert->ntargets-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Target>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Assessment"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Assessment\n");
#endif
          state->state = PARSER_ALERT_ASSESSMENT;
          state->prevstate = PARSER_ALERT;
          alert->assessment = i_new0 (IDMEFassessment, 1);

          if (!alert->assessment)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            alert->assessment->idmefp =
               newAssessment (NULL);

            /* combine the assessment with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->assessment->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <Assessment>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "AdditionalData"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert AdditionalData\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA;
          state->prevstate = PARSER_ALERT;
          alert->nadditionaldatas++;
          ptr = i_renew (IDMEFadditionaldata*,
                                     alert->additionaldatas,
          			     alert->nadditionaldatas);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          alert->additionaldatas = ptr;
          alert->additionaldatas[alert->nadditionaldatas-1] =
            i_new0 (IDMEFadditionaldata, 1);
          
          if (!alert->additionaldatas[alert->nadditionaldatas-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            alert->additionaldatas[alert->nadditionaldatas-1]->idmefp =
               newAdditionalData (NULL);

            /* combine the additional data with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->additionaldatas[alert->nadditionaldatas-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      alert->additionaldatas[alert->nadditionaldatas-1]->type =
                 get_additionaldata_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->additionaldatas[alert->nadditionaldatas-1]->idmefp)
                  alert->additionaldatas[alert->nadditionaldatas-1]->idmefp =
                    newAdditionalData (newAdditionalData_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAdditionalData_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->additionaldatas[alert->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    alert->additionaldatas[alert->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "meaning"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "meaning = %s\n", attrs[i+1]);
#endif
	      alert->additionaldatas[alert->nadditionaldatas-1]->meaning =
                 strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!alert->additionaldatas[alert->nadditionaldatas-1]->idmefp)
                  alert->additionaldatas[alert->nadditionaldatas-1]->idmefp =
                    newAdditionalData (newAdditionalData_meaning ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAdditionalData_meaning ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (alert->additionaldatas[alert->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    alert->additionaldatas[alert->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <AdditionalData>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "ToolAlert"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert ToolAlert\n");
#endif
          state->state = PARSER_ALERT_TOOLALERT;
          state->prevstate = PARSER_ALERT;
          alert->toolalert = i_new0 (IDMEFtoolalert, 1);

          if (!alert->toolalert)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            alert->toolalert->idmefp = newToolAlert (NULL);

            /* combine the assessment with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->toolalert->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <ToolAlert>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "OverflowAlert"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert OverflowAlert\n");
#endif
          state->state = PARSER_ALERT_OVERFLOWALERT;
          state->prevstate = PARSER_ALERT;
          alert->overflowalert = i_new0 (IDMEFoverflowalert, 1);

          if (!alert->overflowalert)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            alert->overflowalert->idmefp =
               newOverflowAlert (NULL);

            /* combine the assessment with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->overflowalert->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <OverflowAlert>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "CorrelationAlert"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert CorrelationAlert\n");
#endif
          state->state = PARSER_ALERT_CORRELATIONALERT;
          state->prevstate = PARSER_ALERT;
          alert->correlationalert = i_new0 (IDMEFcorrelationalert, 1);

          if (!alert->correlationalert)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            alert->correlationalert->idmefp =
               newCorrelationAlert (NULL);

            /* combine the assessment with the alert node */
            libidmef_list_add_element (alert->idmefp, alert->correlationalert->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <CorrelationAlert>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <Analyzer|CreateTime|DetectTime|AnalyzerTime|"
                     "Source|Target|Classification|Assessment|AdditionalData|"
                     "ToolAlert|OverflowAlert|CorrelationAlert>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
    case PARSER_ALERT_ADDITIONALDATA:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFadditionaldata *additionaldata =
          alert->additionaldatas[alert->nadditionaldatas-1];
        
        if (!strcmp ((char*)name, "boolean"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata boolean\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_BOOLEAN;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->boolean = i_new0 (IDMEFboolean, 1);

          if (!additionaldata->boolean)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->boolean->idmefp = newAdditionalData_boolean (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->boolean->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <boolean>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "byte"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata byte\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_BYTE;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->byte = i_new0 (IDMEFbyte, 1);

          if (!additionaldata->byte)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->byte->idmefp = newAdditionalData_byte (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->byte->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <byte>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "character"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata character\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_CHARACTER;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->character = i_new0 (IDMEFcharacter, 1);

          if (!additionaldata->character)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->character->idmefp = newAdditionalData_character (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->character->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <character>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "date-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata date-time\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_DATE_TIME;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->date_time = i_new0 (IDMEFdate_time, 1);

          if (!additionaldata->date_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->date_time->idmefp = newAdditionalData_date_time (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->date_time->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <date-time>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "integer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata integer\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_INTEGER;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->integer = i_new0 (IDMEFinteger, 1);

          if (!additionaldata->integer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->integer->idmefp = newAdditionalData_integer (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->integer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <integer>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "ntpstamp"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata ntpstamp\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_NTPSTAMP;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->ntpstamp = i_new0 (IDMEFntpstamp, 1);

          if (!additionaldata->ntpstamp)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->ntpstamp->idmefp = newAdditionalData_ntpstamp (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->ntpstamp->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <ntpstamp>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "portlist"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata portlist\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_PORTLIST;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->portlist = i_new0 (IDMEFportlist, 1);

          if (!additionaldata->portlist)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->portlist->idmefp = newAdditionalData_portlist (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->portlist->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <portlist>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "real"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata real\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_REAL;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->real = i_new0 (IDMEFreal, 1);

          if (!additionaldata->real)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->real->idmefp = newAdditionalData_real (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->real->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <real>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "string"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata string\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_STRING;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->string = i_new0 (IDMEFstring, 1);

          if (!additionaldata->string)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->string->idmefp = newAdditionalData_string (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->string->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <string>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "byte_string"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata byte_string\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_BYTE_STRING;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->byte_string = i_new0 (IDMEFbyte_string, 1);

          if (!additionaldata->byte_string)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->byte_string->idmefp = newAdditionalData_byte_string (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->byte_string->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <byte-string>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "xmltext"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata xmltext\n");
#endif
          state->state = PARSER_ALERT_ADDITIONALDATA_XMLTEXT;
          state->prevstate = PARSER_ALERT_ADDITIONALDATA;
          additionaldata->xmltext = i_new0 (IDMEFxmltext, 1);

          if (!additionaldata->xmltext)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->xmltext->idmefp = newAdditionalData_xmltext (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->xmltext->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <xmltext>",
                      attrs[i], attrs[i+1]);
	   }
         }

        break;
       }
       
    case PARSER_ALERT_ANALYZER:
       {
        IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);

        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE;
          state->prevstate = PARSER_ALERT_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS;
          state->prevstate = PARSER_ALERT_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newProcess (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Analyzer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer\n");
#endif
//          state->state = PARSER_ALERT_ANALYZER_ANALYZER;
          state->state = PARSER_ALERT_ANALYZER;
          state->prevstate = PARSER_ALERT_ANALYZER;
          state->analyzer_count++;          
          analyzer->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!analyzer->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);
           } 
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process|Analyzer>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       } 
    case PARSER_ALERT_ANALYZER_NODE:
       {
        IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
        IDMEFnode *node = analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node location\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_ALERT_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node name\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_ALERT_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp = newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node Address\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_ALERT_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       } 
    case PARSER_ALERT_ANALYZER_NODE_ADDRESS:
       {
        IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
        IDMEFnode *node = analyzer->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ALERT_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Node Address address\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ALERT_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_ANALYZER_PROCESS:
       {
        IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
        IDMEFprocess *process = analyzer->process;

        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process name\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_ALERT_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process pid\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_ALERT_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process path\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_ALERT_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process arg\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_ALERT_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);
          
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Process env\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_ALERT_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#if 0
    case PARSER_ALERT_ANALYZER_ANALYZER:
       {
        IDMEFanalyzer *analyzer =
                state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer;
                
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newNode (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE:
       {
        IDMEFnode *node =
           state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node location\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node name\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node Address\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS:
       {
        IDMEFnode *node =
          state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Node Address address\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS:
       {
        IDMEFprocess *process =
          state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process name\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process pid\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process path\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process arg\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Analyzer Analyzer Process env\n");
#endif
          state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#endif /* if 0 */       
    case PARSER_ALERT_SOURCE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFsource *source = alert->sources[alert->nsources-1];
        
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE;
          state->prevstate = PARSER_ALERT_SOURCE;
          source->node = i_new0 (IDMEFnode, 1);

          if (!source->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    source->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (source->idmefp, source->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
           
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              source->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->node->idmefp)
                  source->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              source->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->node->idmefp)
                  source->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "User"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source User\n");
#endif
          state->state = PARSER_ALERT_SOURCE_USER;
          state->prevstate = PARSER_ALERT_SOURCE;
          source->user = i_new0 (IDMEFuser, 1);

          if (!source->user)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    source->user->idmefp = newUser (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (source->idmefp, source->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              source->user->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->user->idmefp)
                  source->user->idmefp =
                    newUser (newUser_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->user->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              source->user->category = get_user_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->user->idmefp)
                  source->user->idmefp =
                    newUser (newUser_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->user->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <User>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS;
          state->prevstate = PARSER_ALERT_SOURCE;
          source->process = i_new0 (IDMEFprocess, 1);

          if (!source->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    source->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (source->idmefp, source->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              source->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->process->idmefp)
                  source->process->idmefp =
                    newProcess (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->process->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Service"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE;
          state->prevstate = PARSER_ALERT_SOURCE;
          source->service = i_new0 (IDMEFservice, 1);

          if (!source->service)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            source->service->idmefp =
               newService (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (source->idmefp, source->service->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              source->service->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->service->idmefp)
                  source->service->idmefp =
                    newService (newService_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ip_version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ip_version = %s\n", attrs[i+1]);
#endif
              source->service->ip_version = strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!source->service->idmefp)
                  source->service->idmefp =
                    newService (newService_ip_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ip_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_number"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_number = %s\n", attrs[i+1]);
#endif
              source->service->iana_protocol_number =
                    strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!source->service->idmefp)
                  source->service->idmefp =
                    newService (newService_iana_protocol_number ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_number ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_name = %s\n", attrs[i+1]);
#endif
              source->service->iana_protocol_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!source->service->idmefp)
                  source->service->idmefp =
                    newService (newService_iana_protocol_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (source->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (source->idmefp, source->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Service>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|User|Process|Service>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_NODE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFnode *node = alert->sources[alert->nsources-1]->node;
        
        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node location\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE_LOCATION;
          state->prevstate = PARSER_ALERT_SOURCE_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node name\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE_NAME;
          state->prevstate = PARSER_ALERT_SOURCE_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node Address\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE_ADDRESS;
          state->prevstate = PARSER_ALERT_SOURCE_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);

          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num =
                strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }        
    case PARSER_ALERT_SOURCE_NODE_ADDRESS:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFnode *node = alert->sources[alert->nsources-1]->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node Address netmask\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ALERT_SOURCE_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Node Address address\n");
#endif
          state->state = PARSER_ALERT_SOURCE_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ALERT_SOURCE_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_PROCESS:
       {
        IDMEFalert *alert =
          state->msg->alerts[state->msg->nalerts-1];
        IDMEFsource *source = alert->sources[alert->nsources-1];
        IDMEFprocess *process = source->process;
          
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process name\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS_NAME;
          state->prevstate = PARSER_ALERT_SOURCE_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process pid\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS_PID;
          state->prevstate = PARSER_ALERT_SOURCE_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process path\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS_PATH;
          state->prevstate = PARSER_ALERT_SOURCE_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process arg\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS_ARG;
          state->prevstate = PARSER_ALERT_SOURCE_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Process env\n");
#endif
          state->state = PARSER_ALERT_SOURCE_PROCESS_ENV;
          state->prevstate = PARSER_ALERT_SOURCE_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_USER:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFuser *user = alert->sources[alert->nsources-1]->user;
        
        if (!strcmp ((char*)name, "UserId"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source User UserId\n");
#endif

          state->state = PARSER_ALERT_SOURCE_USER_USERID;
          state->prevstate = PARSER_ALERT_SOURCE_USER;
          user->nuserids++;
          ptr = i_renew (IDMEFuserid*, user->userids,
                                   user->nuserids);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          user->userids = ptr;
          user->userids[user->nuserids-1] = i_new0 (IDMEFuserid, 1);

          if (!user->userids[user->nuserids-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            user->userids[user->nuserids-1]->idmefp =
               newUserId (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->type =
                get_userid_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "tty"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "tty = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->tty = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_tty ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_tty ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <UserId>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <UserId>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_USER_USERID:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFuser *user = alert->sources[alert->nsources-1]->user;
        IDMEFuserid *userid = user->userids[user->nuserids-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source User Userid name\n");
#endif
          state->state = PARSER_ALERT_SOURCE_USER_USERID_NAME;
          state->prevstate = PARSER_ALERT_SOURCE_USER_USERID;
          userid->name = i_new0 (IDMEFname, 1);

          if (!userid->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->name->idmefp = newUserId_name (NULL);

            /* combine the name with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source User Userid number\n");
#endif
          state->state = PARSER_ALERT_SOURCE_USER_USERID_NUMBER;
          state->prevstate = PARSER_ALERT_SOURCE_USER_USERID;
          userid->number = i_new0 (IDMEFnumber, 1);

          if (!userid->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->number->idmefp = newUserId_number (NULL);

            /* combine the number with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|number>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_SERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFservice *service = alert->sources[alert->nsources-1]->service;

        if (!strcmp ((char*)name, "SNMPService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->snmpservice = i_new0 (IDMEFsnmpservice, 1);

          if (!service->snmpservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->snmpservice->idmefp =
               newSNMPService (NULL);

            /* combine the snmpservice with the node node */
            libidmef_list_add_element (service->idmefp, service->snmpservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <SNMPService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "WebService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service WebService\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->webservice = i_new0 (IDMEFwebservice, 1);

          if (!service->webservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->webservice->idmefp =
               newWebService (NULL);

            /* combine the webservice with the node node */
            libidmef_list_add_element (service->idmefp, service->webservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <WebService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service name\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_NAME;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->name = i_new0 (IDMEFname, 1);

          if (!service->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->name->idmefp =
               newService_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (service->idmefp, service->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "port"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service port\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_PORT;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->port = i_new0 (IDMEFport, 1);

          if (!service->port)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->port->idmefp =
               newService_port (NULL);

            /* combine the port with the node node */
            libidmef_list_add_element (service->idmefp, service->port->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <port>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "portlist"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service portlist\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_PORTLIST;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->portlist = i_new0 (IDMEFportlist, 1);

          if (!service->portlist)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->portlist->idmefp =
               newService_portlist (NULL);

            /* combine the portlist with the node node */
            libidmef_list_add_element (service->idmefp, service->portlist->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <portlist>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "protocol"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service protocol\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_PROTOCOL;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE;
          service->protocol = i_new0 (IDMEFprotocol, 1);

          if (!service->protocol)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->protocol->idmefp =
               newService_protocol (NULL);

            /* combine the protocol with the node node */
            libidmef_list_add_element (service->idmefp, service->protocol->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <protocol>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <SNMPService|WebService|name|port|portlist|"
          	     "protocol>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFsnmpservice *snmpservice =
          alert->sources[alert->nsources-1]->service->snmpservice;

        if (!strcmp ((char*)name, "oid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService oid\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_OID;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->oid = i_new0 (IDMEFoid, 1);

          if (!snmpservice->oid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->oid->idmefp =
               newSNMPService_oid (NULL);

            /* combine the oid with the node node */
            libidmef_list_add_element (snmpservice->idmefp, snmpservice->oid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <oid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "community"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService community\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMUNITY;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->community = i_new0 (IDMEFcommunity, 1);

          if (!snmpservice->community)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->community->idmefp =
               newSNMPService_community (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->community->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <community>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "securityName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService securityName\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_SECURITYNAME;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->securityName = i_new0 (IDMEFsecurityName, 1);

          if (!snmpservice->securityName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->securityName->idmefp =
               newSNMPService_securityName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->securityName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <securityName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService contextName\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTNAME;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->contextName = i_new0 (IDMEFcontextName, 1);

          if (!snmpservice->contextName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextName->idmefp =
               newSNMPService_contextName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextEngineID"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService contextEngineID\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTENGINEID;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->contextEngineID = i_new0 (IDMEFcontextEngineID, 1);

          if (!snmpservice->contextEngineID)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextEngineID->idmefp =
               newSNMPService_contextEngineID (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextEngineID->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextEngineID>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "command"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service SNMPService command\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMAND;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
          snmpservice->command = i_new0 (IDMEFcommand, 1);

          if (!snmpservice->command)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->command->idmefp =
               newSNMPService_command (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->command->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <command>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <oid|community|securityName|contextName|"
          	     "contextEngineID|command>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFwebservice *webservice =
          alert->sources[alert->nsources-1]->service->webservice;

        if (!strcmp ((char*)name, "url"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service WebService url\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_URL;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
          webservice->url = i_new0 (IDMEFurl, 1);

          if (!webservice->url)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->url->idmefp =
               newWebService_url (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->url->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <url>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "cgi"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service WebService cgi\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_CGI;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
          webservice->cgi = i_new0 (IDMEFcgi, 1);

          if (!webservice->cgi)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->cgi->idmefp =
               newWebService_cgi (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->cgi->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <cgi>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "http-method"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service WebService http-method\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_HTTP_METHOD;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
          webservice->http_method = i_new0 (IDMEFhttp_method, 1);

          if (!webservice->http_method)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->http_method->idmefp =
               newWebService_http_method (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->http_method->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <http-method>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Source Service WebService arg\n");
#endif
          state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_ARG;
          state->prevstate = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
          webservice->nargs++;
          ptr = i_renew (IDMEFarg*, webservice->args, webservice->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          webservice->args = ptr;
          webservice->args[webservice->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!webservice->args[webservice->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->args[webservice->nargs-1]->idmefp =
               newWebService_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (webservice->idmefp, webservice->args[webservice->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <url|cgi|http-method|arg>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE;
          state->prevstate = PARSER_ALERT_TARGET;
          target->node = i_new0 (IDMEFnode, 1);

          if (!target->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
           
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->node->idmefp)
                  target->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              target->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->node->idmefp)
                  target->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "User"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target User\n");
#endif
          state->state = PARSER_ALERT_TARGET_USER;
          state->prevstate = PARSER_ALERT_TARGET;
          target->user = i_new0 (IDMEFuser, 1);

          if (!target->user)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->user->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->user->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->user->idmefp)
                  target->user->idmefp =
                    newNode (newUser_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->user->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              target->user->category = get_user_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->user->idmefp)
                  target->user->idmefp =
                    newNode (newUser_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->user->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <User>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS;
          state->prevstate = PARSER_ALERT_TARGET;
          target->process = i_new0 (IDMEFprocess, 1);

          if (!target->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (target->idmefp, target->process->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->process->idmefp)
                  target->process->idmefp =
                    newNode (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->process->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Service"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE;
          state->prevstate = PARSER_ALERT_TARGET;
          target->service = i_new0 (IDMEFservice, 1);

          if (!target->service)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            target->service->idmefp =
               newService (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->service->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ip_version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ip_version = %s\n", attrs[i+1]);
#endif
              target->service->ip_version = strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_ip_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ip_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_number"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_number = %s\n", attrs[i+1]);
#endif
              target->service->iana_protocol_number =
                strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_iana_protocol_number ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_number ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_name = %s\n", attrs[i+1]);
#endif
              target->service->iana_protocol_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_iana_protocol_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Service>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "File"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE;
          state->prevstate = PARSER_ALERT_TARGET;
          target->nfiles++;
          ptr = i_renew (IDMEFfile*,target->files,
                                     target->nfiles);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          target->files = ptr;
	  target->files[target->nfiles-1] = i_new0 (IDMEFfile, 1);

          if (!target->files[target->nfiles-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->files[target->nfiles-1]->idmefp = newFile (NULL);

            libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->category =
                get_file_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "fstype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "fstype = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->fstype =
                get_file_fstype ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_fstype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_fstype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "file-type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "file-type = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->file_type = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_file_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_file_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <File>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|User|Process|Service|File>,"
                     " got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_NODE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFnode *node = alert->targets[alert->ntargets-1]->node;
        
        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node location\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE_LOCATION;
          state->prevstate = PARSER_ALERT_TARGET_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node name\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE_NAME;
          state->prevstate = PARSER_ALERT_TARGET_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node Address\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE_ADDRESS;
          state->prevstate = PARSER_ALERT_TARGET_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);

          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
	  
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num =
                strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_NODE_ADDRESS:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFnode *node = alert->targets[alert->ntargets-1]->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node Address netmask\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ALERT_TARGET_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Node Address address\n");
#endif
          state->state = PARSER_ALERT_TARGET_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ALERT_TARGET_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_PROCESS:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFprocess *process = target->process;

        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process name\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS_NAME;
          state->prevstate = PARSER_ALERT_TARGET_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process pid\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS_PID;
          state->prevstate = PARSER_ALERT_TARGET_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process path\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS_PATH;
          state->prevstate = PARSER_ALERT_TARGET_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process arg\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS_ARG;
          state->prevstate = PARSER_ALERT_TARGET_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Process env\n");
#endif
          state->state = PARSER_ALERT_TARGET_PROCESS_ENV;
          state->prevstate = PARSER_ALERT_TARGET_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       } 
    case PARSER_ALERT_TARGET_USER:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFuser *user = alert->targets[alert->ntargets-1]->user;
        
        if (!strcmp ((char*)name, "UserId"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target User UserId\n");
#endif

          state->state = PARSER_ALERT_TARGET_USER_USERID;
          state->prevstate = PARSER_ALERT_TARGET_USER;
          user->nuserids++;
          ptr = i_renew (IDMEFuserid*, user->userids, user->nuserids);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          user->userids = ptr;
          user->userids[user->nuserids-1] = i_new0 (IDMEFuserid, 1);

          if (!user->userids[user->nuserids-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            user->userids[user->nuserids-1]->idmefp =
               newUserId (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->ident = strdup ((char*)attrs[i+1]);
              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->type =
                get_userid_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "tty"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "tty = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->tty = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_tty ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_tty ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <UserId>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <UserId>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_USER_USERID:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFuser *user = alert->targets[alert->ntargets-1]->user;
        IDMEFuserid *userid = user->userids[user->nuserids-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target User Userid name\n");
#endif
          state->state = PARSER_ALERT_TARGET_USER_USERID_NAME;
          state->prevstate = PARSER_ALERT_TARGET_USER_USERID;
          userid->name = i_new0 (IDMEFname, 1);

          if (!userid->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->name->idmefp = newUserId_name (NULL);

            /* combine the name with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target User Userid number\n");
#endif
          state->state = PARSER_ALERT_TARGET_USER_USERID_NUMBER;
          state->prevstate = PARSER_ALERT_TARGET_USER_USERID;
          userid->number = i_new0 (IDMEFnumber, 1);

          if (!userid->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->number->idmefp = newUserId_number (NULL);

            /* combine the number with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|number>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_SERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFservice *service = alert->targets[alert->ntargets-1]->service;

        if (!strcmp ((char*)name, "SNMPService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->snmpservice = i_new0 (IDMEFsnmpservice, 1);

          if (!service->snmpservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->snmpservice->idmefp =
               newSNMPService (NULL);

            /* combine the snmpservice with the node node */
            libidmef_list_add_element (service->idmefp, service->snmpservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <SNMPService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "WebService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service WebService\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->webservice = i_new0 (IDMEFwebservice, 1);

          if (!service->webservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->webservice->idmefp =
               newWebService (NULL);

            /* combine the webservice with the node node */
            libidmef_list_add_element (service->idmefp, service->webservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <WebService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service name\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_NAME;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->name = i_new0 (IDMEFname, 1);

          if (!service->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->name->idmefp =
               newService_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (service->idmefp, service->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "port"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service port\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_PORT;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->port = i_new0 (IDMEFport, 1);

          if (!service->port)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->port->idmefp =
               newService_port (NULL);

            /* combine the port with the node node */
            libidmef_list_add_element (service->idmefp, service->port->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <port>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "portlist"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service portlist\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_PORTLIST;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->portlist = i_new0 (IDMEFportlist, 1);

          if (!service->portlist)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->portlist->idmefp =
               newService_portlist (NULL);

            /* combine the portlist with the node node */
            libidmef_list_add_element (service->idmefp, service->portlist->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <portlist>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "protocol"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service protocol\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_PROTOCOL;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE;
          service->protocol = i_new0 (IDMEFprotocol, 1);

          if (!service->protocol)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->protocol->idmefp =
               newService_protocol (NULL);

            /* combine the protocol with the node node */
            libidmef_list_add_element (service->idmefp, service->protocol->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <protocol>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <SNMPService|WebService|name|port|portlist|"
          	     "protocol>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFsnmpservice *snmpservice =
          alert->targets[alert->ntargets-1]->service->snmpservice;

        if (!strcmp ((char*)name, "oid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService oid\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_OID;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->oid = i_new0 (IDMEFoid, 1);

          if (!snmpservice->oid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->oid->idmefp =
               newSNMPService_oid (NULL);

            /* combine the oid with the node node */
            libidmef_list_add_element (snmpservice->idmefp, snmpservice->oid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <oid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "community"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService community\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMUNITY;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->community = i_new0 (IDMEFcommunity, 1);

          if (!snmpservice->community)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->community->idmefp =
               newSNMPService_community (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->community->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <community>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "securityName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService securityName\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->securityName = i_new0 (IDMEFsecurityName, 1);

          if (!snmpservice->securityName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->securityName->idmefp =
               newSNMPService_securityName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->securityName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <securityName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService contextName\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->contextName = i_new0 (IDMEFcontextName, 1);

          if (!snmpservice->contextName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextName->idmefp =
               newSNMPService_contextName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextEngineID"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService contextEngineID\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->contextEngineID = i_new0 (IDMEFcontextEngineID, 1);

          if (!snmpservice->contextEngineID)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextEngineID->idmefp =
               newSNMPService_contextEngineID (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextEngineID->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextEngineID>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "command"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service SNMPService command\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMAND;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->command = i_new0 (IDMEFcommand, 1);

          if (!snmpservice->command)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->command->idmefp =
               newSNMPService_command (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->command->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <command>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <oid|community|securityName|contextName|"
          	     "contextEngineID|command>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFwebservice *webservice =
          alert->targets[alert->ntargets-1]->service->webservice;

        if (!strcmp ((char*)name, "url"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service WebService url\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_URL;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
          webservice->url = i_new0 (IDMEFurl, 1);

          if (!webservice->url)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->url->idmefp =
               newWebService_url (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->url->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <url>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "cgi"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service WebService cgi\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_CGI;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
          webservice->cgi = i_new0 (IDMEFcgi, 1);

          if (!webservice->cgi)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->cgi->idmefp =
               newWebService_cgi (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->cgi->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <cgi>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "http-method"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service WebService http-method\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
          webservice->http_method = i_new0 (IDMEFhttp_method, 1);

          if (!webservice->http_method)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->http_method->idmefp =
               newWebService_http_method (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->http_method->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <http-method>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target Service WebService arg\n");
#endif
          state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_ARG;
          state->prevstate = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
          webservice->nargs++;
          ptr = i_renew (IDMEFarg*, webservice->args, webservice->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          webservice->args = ptr;
          webservice->args[webservice->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!webservice->args[webservice->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->args[webservice->nargs-1]->idmefp =
               newWebService_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (webservice->idmefp, webservice->args[webservice->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <url|cgi|http-method|arg>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFfile *file = target->files[target->nfiles-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File name\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_NAME;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->name = i_new0 (IDMEFname, 1);

          if (!file->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->name->idmefp = newFile_name (NULL);

            libidmef_list_add_element (file->idmefp, file->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File path\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_PATH;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->path = i_new0 (IDMEFpath, 1);

          if (!file->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->path->idmefp = newFile_path (NULL);

            libidmef_list_add_element (file->idmefp, file->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "create-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File create-time\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_CREATE_TIME;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->create_time = i_new0 (IDMEFcreate_time, 1);

          if (!file->create_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->create_time->idmefp = newFile_create_time (NULL);

            libidmef_list_add_element (file->idmefp, file->create_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <create-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "modify-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File modify-time\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_MODIFY_TIME;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->modify_time = i_new0 (IDMEFmodify_time, 1);

          if (!file->modify_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->modify_time->idmefp = newFile_modify_time (NULL);

            libidmef_list_add_element (file->idmefp, file->modify_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <modify-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "access-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File access-time\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_ACCESS_TIME;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->access_time = i_new0 (IDMEFaccess_time, 1);

          if (!file->access_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->access_time->idmefp = newFile_access_time (NULL);

            libidmef_list_add_element (file->idmefp, file->access_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <access-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "data-size"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File data-size\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_DATA_SIZE;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->data_size = i_new0 (IDMEFdata_size, 1);

          if (!file->data_size)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->data_size->idmefp = newFile_data_size (NULL);

            libidmef_list_add_element (file->idmefp, file->data_size->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <data-size>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "disk-size"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File disk-size\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_DISK_SIZE;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->disk_size = i_new0 (IDMEFdisk_size, 1);

          if (!file->disk_size)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->disk_size->idmefp = newFile_disk_size (NULL);

            libidmef_list_add_element (file->idmefp, file->disk_size->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <disk-size>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "FileAccess"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File FileAccess\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->nfileaccesses++;
          ptr = i_renew (IDMEFfileaccess*,
                                        file->fileaccesses,
                                        file->nfileaccesses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->fileaccesses = ptr;
	  file->fileaccesses[file->nfileaccesses-1] =
            i_new0 (IDMEFfileaccess, 1);

          if (!file->fileaccesses[file->nfileaccesses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->fileaccesses[file->nfileaccesses-1]->idmefp =
               newFileAccess (NULL);

            /* combine the fileaccess with the file node */
            libidmef_list_add_element (file->idmefp, file->fileaccesses[file->nfileaccesses-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <FileAccess>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Linkage"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Linkage\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_LINKAGE;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->nlinkages++;
          ptr = i_renew (IDMEFlinkage*,
                                    file->linkages, file->nlinkages);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->linkages = ptr;
	  file->linkages[file->nlinkages-1] = i_new0 (IDMEFlinkage, 1);

          if (!file->linkages[file->nlinkages-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            file->linkages[file->nlinkages-1]->idmefp =
               newLinkage (NULL);

            /* combine the linkage with the file node */
            libidmef_list_add_element (file->idmefp, file->linkages[file->nlinkages-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      file->linkages[file->nlinkages-1]->category =
                get_linkage_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!file->linkages[file->nlinkages-1]->idmefp)
                  file->linkages[file->nlinkages-1]->idmefp =
                    newLinkage (newLinkage_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newLinkage_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (file->linkages[file->nlinkages-1]->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (file->idmefp, file->linkages[file->nlinkages-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Linkage>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Inode"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->inode = i_new0 (IDMEFinode, 1);

          if (!file->inode)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->inode->idmefp = newInode (NULL);

            libidmef_list_add_element (file->idmefp, file->inode->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <Inode>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Checksum"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Checksum\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_CHECKSUM;
          state->prevstate = PARSER_ALERT_TARGET_FILE;
          file->nchecksums++;
          ptr = i_renew (IDMEFchecksum*,
                                     file->checksums, file->nchecksums);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->checksums = ptr;
	  file->checksums[file->nchecksums-1] = i_new0 (IDMEFchecksum, 1);

          if (!file->checksums[file->nchecksums-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    file->checksums[file->nchecksums-1]->idmefp = newChecksum (NULL);

            libidmef_list_add_element (file->idmefp, file->checksums[file->nchecksums-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "algorithm"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "algorithm = %s\n", attrs[i+1]);
#endif
	      file->checksums[file->nchecksums-1]->algorithm =
                get_checksum_algorithm ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!file->checksums[file->nchecksums-1]->idmefp)
                  file->checksums[file->nchecksums-1]->idmefp =
                    newChecksum (newChecksum_algorithm ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newChecksum_algorithm ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (file->checksums[file->nchecksums-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (file->idmefp, file->checksums[file->nchecksums-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Checksum>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|path|create-time|modify-time|access-time|"
                     "data-size|disk-size|FileAccess|Linkage|Inode|Checksum>,"
                     " got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE_FILEACCESS:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFfileaccess *fileaccess = file->fileaccesses[file->nfileaccesses-1];
        
        if (!strcmp ((char*)name, "UserId"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File FileAccess UserId\n");
#endif

          state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID;
          state->prevstate = PARSER_ALERT_TARGET_FILE_FILEACCESS;
          fileaccess->userid = i_new0 (IDMEFuserid, 1);

          if (!fileaccess->userid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            fileaccess->userid->idmefp = newUserId (NULL);

            /* combine the userid with the fileaccess node */
            libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              fileaccess->userid->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the userid with the fileaccess node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
              fileaccess->userid->type = get_userid_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the userid with the fileaccess node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "tty"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "tty = %s\n", attrs[i+1]);
#endif
	      fileaccess->userid->tty = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_tty ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_tty ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <UserId>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "permission"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File FileAccess permission\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_PERMISSION;
          state->prevstate = PARSER_ALERT_TARGET_FILE_FILEACCESS;
          fileaccess->npermissions++;
          ptr = i_renew (IDMEFpermission*, fileaccess->permissions,
                                             fileaccess->npermissions);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          fileaccess->permissions = ptr;
          fileaccess->permissions[fileaccess->npermissions-1] =
            i_new0 (IDMEFpermission, 1);
          
          if (!fileaccess->permissions[fileaccess->npermissions-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            fileaccess->permissions[fileaccess->npermissions-1]->idmefp =
              newFileAccess_permission (NULL);

            /* combine the  with the  node */
            libidmef_list_add_element (fileaccess->idmefp, fileaccess->permissions[fileaccess->npermissions-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <permission>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <UserId|permission>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFuserid *userid = file->fileaccesses[file->nfileaccesses-1]->userid;
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File FileAccess Userid name\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NAME;
          state->prevstate = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID;
          userid->name = i_new0 (IDMEFname, 1);

          if (!userid->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->name->idmefp = newUserId_name (NULL);

            /* combine the name with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File FileAccess Userid number\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NUMBER;
          state->prevstate = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID;
          userid->number = i_new0 (IDMEFnumber, 1);

          if (!userid->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->number->idmefp = newUserId_number (NULL);

            /* combine the number with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|number>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE_LINKAGE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFlinkage *linkage = file->linkages[file->nlinkages-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Linkage name\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_LINKAGE_NAME;
          state->prevstate = PARSER_ALERT_TARGET_FILE_LINKAGE;
          linkage->name = i_new0 (IDMEFname, 1);

          if (!linkage->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            linkage->name->idmefp = newLinkage_name (NULL);

            /* combine the name with the linkage node */
            libidmef_list_add_element (linkage->idmefp, linkage->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Linkage path\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_LINKAGE_PATH;
          state->prevstate = PARSER_ALERT_TARGET_FILE_LINKAGE;
          linkage->path = i_new0 (IDMEFpath, 1);

          if (!linkage->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            linkage->path->idmefp = newLinkage_path (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (linkage->idmefp, linkage->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|path>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE_INODE:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFinode *inode = target->files[target->nfiles-1]->inode;

        if (!strcmp ((char*)name, "change-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode change-time\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_CHANGE_TIME;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->change_time = i_new0 (IDMEFchange_time, 1);

          if (!inode->change_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->change_time->idmefp = newInode_change_time (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->change_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <change-time>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode number\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_NUMBER;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->number = i_new0 (IDMEFnumber, 1);

          if (!inode->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->number->idmefp = newInode_number (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "major-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode major-device\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_MAJOR_DEVICE;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->major_device = i_new0 (IDMEFmajor_device, 1);

          if (!inode->major_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->major_device->idmefp = newInode_major_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->major_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <major-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "minor-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode minor-device\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_MINOR_DEVICE;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->minor_device = i_new0 (IDMEFminor_device, 1);

          if (!inode->minor_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->minor_device->idmefp = newInode_minor_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->minor_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <minor-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "c-major-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode c-major-device\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_C_MAJOR_DEVICE;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->c_major_device = i_new0 (IDMEFc_major_device, 1);

          if (!inode->c_major_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->c_major_device->idmefp = newInode_c_major_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->c_major_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <c-major-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "c-minor-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Inode c-minor-device\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_INODE_C_MINOR_DEVICE;
          state->prevstate = PARSER_ALERT_TARGET_FILE_INODE;
          inode->c_minor_device = i_new0 (IDMEFc_minor_device, 1);

          if (!inode->c_minor_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->c_minor_device->idmefp = newInode_c_minor_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->c_minor_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <c-minor-device>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <change-time|number|major-device|minor-device|"
                     "c-major-device|c-minor-device>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TARGET_FILE_CHECKSUM:
       {
        IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
        IDMEFtarget *target = alert->targets[alert->ntargets-1];
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFchecksum *checksum = file->checksums[file->nchecksums-1];

        if (!strcmp ((char*)name, "value"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Checksum value\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_CHECKSUM_VALUE;
          state->prevstate = PARSER_ALERT_TARGET_FILE_CHECKSUM;
          checksum->value = i_new0 (IDMEFvalue, 1);

          if (!checksum->value)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            checksum->value->idmefp =
               newChecksum_value (NULL);

            libidmef_list_add_element (checksum->idmefp, checksum->value->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <value>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "key"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Target File Checksum key\n");
#endif
          state->state = PARSER_ALERT_TARGET_FILE_CHECKSUM_KEY;
          state->prevstate = PARSER_ALERT_TARGET_FILE_CHECKSUM;
          checksum->key = i_new0 (IDMEFkey, 1);

          if (!checksum->key)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            checksum->key->idmefp =
               newChecksum_key (NULL);

            libidmef_list_add_element (checksum->idmefp, checksum->key->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <key>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <value|key>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_CLASSIFICATION:
       {
        IDMEFclassification *classification =
          state->msg->alerts[state->msg->nalerts-1]->classification;
        
        if (!strcmp ((char*)name, "Reference"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Classification Reference\n");
#endif
          state->state = PARSER_ALERT_CLASSIFICATION_REFERENCE;
          state->prevstate = PARSER_ALERT_CLASSIFICATION;
          classification->nreferences++;
          ptr = i_renew (IDMEFreference*,
                               classification->references,
                               classification->nreferences);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          classification->references = ptr;
	  classification->references[classification->nreferences-1] =
            i_new0 (IDMEFreference, 1);

          if (!classification->references[classification->nreferences-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    classification->references[classification->nreferences-1]->idmefp =
              newReference (NULL);

            libidmef_list_add_element (classification->idmefp, classification->references[classification->nreferences-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "origin"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "origin = %s\n", attrs[i+1]);
#endif
	      classification->references[classification->nreferences-1]->origin =
                get_reference_origin ((char*)attrs[i+1]);
  
              if (state->gen_idmefp)
               {
                if (!classification->references[classification->nreferences-1]->idmefp)
                  classification->references[classification->nreferences-1]->idmefp =
                    newReference (newReference_origin ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newReference_origin ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (classification->references[classification->nreferences-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (classification->idmefp, classification->references[classification->nreferences-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
           }
            else if (!strcmp((char*)attrs[i], "meaning"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "meaning = %s\n", attrs[i+1]);
#endif
	      classification->references[classification->nreferences-1]->meaning =
                strdup ((char*)attrs[i+1]);
  
              if (state->gen_idmefp)
               {
                if (!classification->references[classification->nreferences-1]->idmefp)
                  classification->references[classification->nreferences-1]->idmefp =
                    newReference (newReference_meaning ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newReference_meaning ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (classification->references[classification->nreferences-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (classification->idmefp, classification->references[classification->nreferences-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Reference>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Reference>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_CLASSIFICATION_REFERENCE:
       {
        IDMEFclassification *classification =
          state->msg->alerts[state->msg->nalerts-1]->classification;
        IDMEFreference *reference =
          classification->references[classification->nreferences-1];
          
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Classification Reference name\n");
#endif
          state->state = PARSER_ALERT_CLASSIFICATION_REFERENCE_NAME;
          state->prevstate = PARSER_ALERT_CLASSIFICATION_REFERENCE;
          reference->name = i_new0 (IDMEFname, 1);

          if (!reference->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            reference->name->idmefp = newReference_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (reference->idmefp, reference->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "url"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Classification Reference url\n");
#endif
          state->state =PARSER_ALERT_CLASSIFICATION_REFERENCE_URL;
          state->prevstate = PARSER_ALERT_CLASSIFICATION_REFERENCE;
          reference->url = i_new0 (IDMEFurl, 1);

          if (!reference->url)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            reference->url->idmefp = newReference_url (NULL);

            /* combine the url with the node node */
            libidmef_list_add_element (reference->idmefp, reference->url->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <url>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|url>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_ASSESSMENT:
       {
        IDMEFassessment *assessment =
          state->msg->alerts[state->msg->nalerts-1]->assessment;
          
        if (!strcmp ((char*)name, "Impact"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Assessment Impact\n");
#endif
          state->state = PARSER_ALERT_ASSESSMENT_IMPACT;
          state->prevstate = PARSER_ALERT_ASSESSMENT;
          assessment->impact = i_new0 (IDMEFimpact, 1);

          if (!assessment->impact)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    assessment->impact->idmefp = newImpact (NULL);

            libidmef_list_add_element (assessment->idmefp, assessment->impact->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "severity"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "severity = %s\n", attrs[i+1]);
#endif
	      assessment->impact->severity = get_impact_severity ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!assessment->impact->idmefp)
                  assessment->impact->idmefp =
                    newImpact ("", newImpact_severity ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newImpact_severity ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (assessment->impact->idmefp, tidmefp);
                 }

                libidmef_list_add_element (assessment->idmefp, assessment->impact->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "completion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "completion = %s\n", attrs[i+1]);
#endif
	      assessment->impact->completion = get_impact_completion ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!assessment->impact->idmefp)
                  assessment->impact->idmefp =
                    newImpact ("", newImpact_completion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newImpact_completion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (assessment->impact->idmefp, tidmefp);
                 }

                libidmef_list_add_element (assessment->idmefp, assessment->impact->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      assessment->impact->type = get_impact_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!assessment->impact->idmefp)
                  assessment->impact->idmefp =
                    newImpact ("", newImpact_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newImpact_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (assessment->impact->idmefp, tidmefp);
                 }

                libidmef_list_add_element (assessment->idmefp, assessment->impact->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Impact>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Action"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Assessment Action\n");
#endif
          state->state = PARSER_ALERT_ASSESSMENT_ACTION;
          state->prevstate = PARSER_ALERT_ASSESSMENT;
          assessment->nactions++;
          ptr = i_renew (IDMEFaction*, assessment->actions,
          				 assessment->nactions);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          assessment->actions = ptr;
	  assessment->actions[assessment->nactions-1] = i_new0 (IDMEFaction, 1);

          if (!assessment->actions[assessment->nactions-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    assessment->actions[assessment->nactions-1]->idmefp = newAction (NULL);

            libidmef_list_add_element (assessment->idmefp, assessment->actions[assessment->nactions-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              assessment->actions[assessment->nactions-1]->category = 
                get_action_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!assessment->actions[assessment->nactions-1]->idmefp)
                  assessment->actions[assessment->nactions-1]->idmefp =
                    newAction ("", newAction_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAction_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (assessment->actions[assessment->nactions-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (assessment->idmefp, assessment->actions[assessment->nactions-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Action>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Confidence"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Assessment Confidence\n");
#endif
          state->state = PARSER_ALERT_ASSESSMENT_CONFIDENCE;
          state->prevstate = PARSER_ALERT_ASSESSMENT;
          assessment->confidence = i_new0 (IDMEFconfidence, 1);

          if (!assessment->confidence)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    assessment->confidence->idmefp = newConfidence (NULL);

            libidmef_list_add_element (assessment->idmefp, assessment->confidence->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "rating"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "rating = %s\n", attrs[i+1]);
#endif
	      assessment->confidence->rating =
                get_confidence_rating ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!assessment->confidence->idmefp)
                  assessment->confidence->idmefp =
                    newConfidence ("", newConfidence_rating ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newConfidence_rating ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (assessment->confidence->idmefp, tidmefp);
                 }

                libidmef_list_add_element (assessment->idmefp, assessment->confidence->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Confidence>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Impact|Action|Confidence>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_TOOLALERT:
       {
        IDMEFtoolalert *toolalert =
          state->msg->alerts[state->msg->nalerts-1]->toolalert;
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Toolalert name\n");
#endif
          state->state = PARSER_ALERT_TOOLALERT_NAME;
          state->prevstate = PARSER_ALERT_TOOLALERT;
          toolalert->name = i_new0 (IDMEFname, 1);

          if (!toolalert->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            toolalert->name->idmefp = newToolAlert_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (toolalert->idmefp, toolalert->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "command"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Toolalert command\n");
#endif
          state->state = PARSER_ALERT_TOOLALERT_COMMAND;
          state->prevstate = PARSER_ALERT_TOOLALERT;
          toolalert->command = i_new0 (IDMEFcommand, 1);

          if (!toolalert->command)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            toolalert->command->idmefp = newToolAlert_command (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (toolalert->idmefp, toolalert->command->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <command>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "alertident"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Toolalert alertident\n");
#endif
          state->state = PARSER_ALERT_TOOLALERT_ALERTIDENT;
          state->prevstate = PARSER_ALERT_TOOLALERT;
          toolalert->nalertidents++;
          ptr = i_renew (IDMEFalertident*,
          		toolalert->alertidents, toolalert->nalertidents);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          toolalert->alertidents = ptr;
          toolalert->alertidents[toolalert->nalertidents-1] =
            i_new0 (IDMEFalertident, 1);

          if (!toolalert->alertidents[toolalert->nalertidents-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    toolalert->alertidents[toolalert->nalertidents-1]->idmefp =
              newToolAlert_alertident (NULL);

            libidmef_list_add_element (toolalert->idmefp, toolalert->alertidents[toolalert->nalertidents-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              toolalert->alertidents[toolalert->nalertidents-1]->analyzerid =
                strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!toolalert->alertidents[toolalert->nalertidents-1]->idmefp)
                  toolalert->alertidents[toolalert->nalertidents-1]->idmefp =
                     newToolAlert (newToolAlert_alertident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newToolAlert_alertident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (toolalert->alertidents[toolalert->nalertidents-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (toolalert->idmefp, toolalert->alertidents[toolalert->nalertidents-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <alertident>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|command|alertident>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_OVERFLOWALERT:
       {
        IDMEFoverflowalert *overflowalert =
          state->msg->alerts[state->msg->nalerts-1]->overflowalert;
        
        if (!strcmp ((char*)name, "program"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert OverflowAlert program\n");
#endif
          state->state = PARSER_ALERT_OVERFLOWALERT_PROGRAM;
          state->prevstate = PARSER_ALERT_OVERFLOWALERT;
          overflowalert->program = i_new0 (IDMEFprogram, 1);

          if (!overflowalert->program)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            overflowalert->program->idmefp = newOverflowAlert_program (NULL);

            libidmef_list_add_element (overflowalert->idmefp, overflowalert->program->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <program>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "size"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert OverflowAlert size\n");
#endif
          state->state = PARSER_ALERT_OVERFLOWALERT_SIZE;
          state->prevstate = PARSER_ALERT_OVERFLOWALERT;
          overflowalert->size = i_new0 (IDMEFsize, 1);

          if (!overflowalert->size)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            overflowalert->size->idmefp = newOverflowAlert_size (NULL);

            libidmef_list_add_element (overflowalert->idmefp, overflowalert->size->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <size>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "buffer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert OverflowAlert buffer\n");
#endif
          state->state = PARSER_ALERT_OVERFLOWALERT_BUFFER;
          state->prevstate = PARSER_ALERT_OVERFLOWALERT;
          overflowalert->buffer = i_new0 (IDMEFbuffer, 1);

          if (!overflowalert->buffer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            overflowalert->buffer->idmefp = newOverflowAlert_buffer (NULL);

            libidmef_list_add_element (overflowalert->idmefp, overflowalert->buffer->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <program|size|buffer>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ALERT_CORRELATIONALERT:
       {
        IDMEFcorrelationalert *correlationalert =
          state->msg->alerts[state->msg->nalerts-1]->correlationalert;
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert CorrelationAlert name\n");
#endif
          state->state = PARSER_ALERT_CORRELATIONALERT_NAME;
          state->prevstate = PARSER_ALERT_CORRELATIONALERT;
          correlationalert->name = i_new0 (IDMEFname, 1);

          if (!correlationalert->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            correlationalert->name->idmefp = newCorrelationAlert_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (correlationalert->idmefp, correlationalert->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "alertident"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert CorrelationAlert alertident\n");
#endif
          state->state = PARSER_ALERT_CORRELATIONALERT_ALERTIDENT;
          state->prevstate = PARSER_ALERT_CORRELATIONALERT;
          correlationalert->nalertidents++;
          ptr = i_renew (IDMEFalertident*,
          		correlationalert->alertidents, correlationalert->nalertidents);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          correlationalert->alertidents = ptr;
          correlationalert->alertidents[correlationalert->nalertidents-1] =
            i_new0 (IDMEFalertident, 1);

          if (!correlationalert->alertidents[correlationalert->nalertidents-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp =
              newCorrelationAlert_alertident (NULL);

            libidmef_list_add_element (correlationalert->idmefp, correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              correlationalert->alertidents[correlationalert->nalertidents-1]->analyzerid =
                strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp)
                  correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp =
                     newCorrelationAlert (
                       newCorrelationAlert_alertident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newCorrelationAlert_alertident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (correlationalert->idmefp, correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <alertident>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|alertident>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_HEARTBEAT:
       {
        IDMEFheartbeat *heartbeat =
           state->msg->heartbeats[state->msg->nheartbeats-1];
        
    	if (!strcmp ((char*)name, "Analyzer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER;
          state->prevstate = PARSER_HEARTBEAT;
          state->analyzer_count++;
          heartbeat->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!heartbeat->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    heartbeat->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the analyzer with the heartbeat node */
            libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzer->idmefp)
                  heartbeat->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);
           } 
         }
        else if (!strcmp ((char*)name, "CreateTime"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat CreateTime\n");
#endif
          state->state = PARSER_HEARTBEAT_CREATETIME;
          state->prevstate = PARSER_HEARTBEAT;
          heartbeat->createtime = i_new0 (IDMEFtime, 1);

          if (!heartbeat->createtime)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    heartbeat->createtime->idmefp =
	        newCreateTime (NULL);

            /* combine the createtime with the heartbeat node */
            libidmef_list_add_element (heartbeat->idmefp, heartbeat->createtime->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ntpstamp"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ntpstamp = %s\n", attrs[i+1]);
#endif
              heartbeat->createtime->ntpstamp = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->createtime->idmefp)
                  heartbeat->createtime->idmefp =
                    newCreateTime ("", newCreateTime_ntpstamp ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newCreateTime_ntpstamp ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->createtime->idmefp, tidmefp);
                 }

                /* combine the createtime with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->createtime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <CreateTime>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "AnalyzerTime"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat AnalyzerTime\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZERTIME;
          state->prevstate = PARSER_HEARTBEAT;
          heartbeat->analyzertime = i_new0 (IDMEFtime, 1);

          if (!heartbeat->analyzertime)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    heartbeat->analyzertime->idmefp =
	        newAnalyzerTime (NULL);

            /* combine the analyzertime with the heartbeat node */
            libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzertime->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ntpstamp"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ntpstamp = %s\n", attrs[i+1]);
#endif
              heartbeat->analyzertime->ntpstamp = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->analyzertime->idmefp)
                  heartbeat->analyzertime->idmefp =
                    newAnalyzerTime ("", newAnalyzerTime_ntpstamp ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzerTime_ntpstamp ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->analyzertime->idmefp, tidmefp);
                 }

                /* combine the detecttime with the heartbeat node */
                libidmef_list_add_element (heartbeat->idmefp, heartbeat->analyzertime->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	     }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <AnalyzerTime>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "AdditionalData"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat AdditionalData\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
          state->prevstate = PARSER_HEARTBEAT;
          heartbeat->nadditionaldatas++;
          ptr = i_renew (IDMEFadditionaldata*,
                                     heartbeat->additionaldatas,
          			     heartbeat->nadditionaldatas);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          heartbeat->additionaldatas = ptr;
          heartbeat->additionaldatas[heartbeat->nadditionaldatas-1] =
            i_new0 (IDMEFadditionaldata, 1);
            
          if (!heartbeat->additionaldatas[heartbeat->nadditionaldatas-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp =
               newAdditionalData (NULL);

            /* combine the additional data with the heartbeat node */
            libidmef_list_add_element (heartbeat->idmefp, heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->type =
                 get_additionaldata_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp)
                  heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp =
                    newAdditionalData (newAdditionalData_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAdditionalData_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the heartbeat with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "meaning"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "meaning = %s\n", attrs[i+1]);
#endif
	      heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->meaning =
                 strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp)
                  heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp =
                    newAdditionalData (newAdditionalData_meaning ((char*)attrs[i+1], NULL),
                                       NULL);
                else
                 {
                  tidmefp =  newAdditionalData_meaning ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the heartbeat with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    heartbeat->additionaldatas[heartbeat->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <AdditionalData>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "HeartbeatInterval"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat HeartbeatInterval\n");
#endif
          state->state = PARSER_HEARTBEAT_HEARTBEATINTERVAL;
          state->prevstate = PARSER_HEARTBEAT;
          heartbeat->heartbeatinterval = i_new0 (IDMEFheartbeatinterval, 1);

          if (!heartbeat->heartbeatinterval)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            heartbeat->heartbeatinterval->idmefp =
               newHeartbeatInterval (NULL);

            /* combine the assessment with the heartbeat node */
            libidmef_list_add_element (heartbeat->idmefp, heartbeat->heartbeatinterval->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[0])
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <HeartbeatInterval>",
                        attrs[0], attrs[1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <Analyzer|CreateTime|AdditionalData|HeartbeatInterval>,"
                     " got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
    case PARSER_HEARTBEAT_ANALYZER:
       {
        IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
                
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newProcess (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Analyzer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer\n");
#endif
//          state->state = PARSER_HEARTBEAT_ANALYZER;
          state->state = PARSER_HEARTBEAT_ANALYZER;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER;
          state->analyzer_count++;          
          analyzer->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!analyzer->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);
           } 
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process|Analyzer>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
    case PARSER_HEARTBEAT_ANALYZER_NODE:
       {
        IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
        IDMEFnode *node = analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node location\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node name\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp = newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node Address\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS:
       {
        IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
        IDMEFnode *node = analyzer->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Node Address address\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
    case PARSER_HEARTBEAT_ANALYZER_PROCESS:
       {
        IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
        IDMEFprocess *process = analyzer->process;

        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process name\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process pid\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process path\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process arg\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Process env\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#if 0
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER:
       {
        IDMEFanalyzer *analyzer =
          state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer;
                
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the alert with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newNode (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE:
       {
        IDMEFnode *node =
           state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node location\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node name\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node Address\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS:
       {
        IDMEFnode *node =
          state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Node Address address\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS:
       {
        IDMEFprocess *process =
          state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process name\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process pid\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process path\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process arg\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Analyzer Analyzer Process env\n");
#endif
          state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#endif /* if 0*/

      case PARSER_HEARTBEAT_ADDITIONALDATA:
       {
        IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
        IDMEFadditionaldata *additionaldata =
          heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];
        
        if (!strcmp ((char*)name, "boolean"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata boolean\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_BOOLEAN;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->boolean = i_new0 (IDMEFboolean, 1);

          if (!additionaldata->boolean)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->boolean->idmefp = newAdditionalData_boolean (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->boolean->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <boolean>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "byte"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata byte\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_BYTE;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->byte = i_new0 (IDMEFbyte, 1);

          if (!additionaldata->byte)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->byte->idmefp = newAdditionalData_byte (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->byte->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <byte>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "character"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata character\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_CHARACTER;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->character = i_new0 (IDMEFcharacter, 1);

          if (!additionaldata->character)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->character->idmefp = newAdditionalData_character (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->character->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <character>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "date-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Heartbeat Additionaldata date-time\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_DATE_TIME;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->date_time = i_new0 (IDMEFdate_time, 1);

          if (!additionaldata->date_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->date_time->idmefp = newAdditionalData_date_time (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->date_time->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <date-time>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "integer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata integer\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_INTEGER;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->integer = i_new0 (IDMEFinteger, 1);

          if (!additionaldata->integer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->integer->idmefp = newAdditionalData_integer (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->integer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <integer>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "ntpstamp"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata ntpstamp\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_NTPSTAMP;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->ntpstamp = i_new0 (IDMEFntpstamp, 1);

          if (!additionaldata->ntpstamp)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->ntpstamp->idmefp = newAdditionalData_ntpstamp (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->ntpstamp->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <ntpstamp>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "portlist"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata portlist\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_PORTLIST;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->portlist = i_new0 (IDMEFportlist, 1);

          if (!additionaldata->portlist)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->portlist->idmefp = newAdditionalData_portlist (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->portlist->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <portlist>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "real"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata real\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_REAL;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->real = i_new0 (IDMEFreal, 1);

          if (!additionaldata->real)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->real->idmefp = newAdditionalData_real (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->real->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <real>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "string"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata string\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_STRING;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->string = i_new0 (IDMEFstring, 1);

          if (!additionaldata->string)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->string->idmefp = newAdditionalData_string (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->string->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <string>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "byte_string"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata byte_string\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_BYTE_STRING;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->byte_string = i_new0 (IDMEFbyte_string, 1);

          if (!additionaldata->byte_string)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->byte_string->idmefp = newAdditionalData_byte_string (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->byte_string->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <byte-string>",
                      attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "xmltext"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Alert Additionaldata xmltext\n");
#endif
          state->state = PARSER_HEARTBEAT_ADDITIONALDATA_XMLTEXT;
          state->prevstate = PARSER_HEARTBEAT_ADDITIONALDATA;
          additionaldata->xmltext = i_new0 (IDMEFxmltext, 1);

          if (!additionaldata->xmltext)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    additionaldata->xmltext->idmefp = newAdditionalData_xmltext (NULL);

            /* combine the heartbeat with the message node */
            libidmef_list_add_element (additionaldata->idmefp,
                                       additionaldata->xmltext->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <xmltext>",
                      attrs[i], attrs[i+1]);
	   }
         }

        break;
       }
       
//########################################################################################
        
      case PARSER_ALERT_ANALYZER_NODE_NAME:
      case PARSER_ALERT_ANALYZER_PROCESS_NAME:
      case PARSER_ALERT_SOURCE_USER_USERID_NAME:
      case PARSER_ALERT_SOURCE_SERVICE_NAME:
      case PARSER_ALERT_SOURCE_NODE_NAME:
      case PARSER_ALERT_SOURCE_PROCESS_NAME:
      case PARSER_ALERT_TARGET_NODE_NAME:
      case PARSER_ALERT_TARGET_PROCESS_NAME:
      case PARSER_ALERT_TARGET_USER_USERID_NAME:
      case PARSER_ALERT_TARGET_SERVICE_NAME:
      case PARSER_ALERT_TARGET_FILE_NAME:
      case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NAME:
      case PARSER_ALERT_TARGET_FILE_LINKAGE_NAME:
      case PARSER_ALERT_CLASSIFICATION_REFERENCE_NAME:
      case PARSER_ALERT_TOOLALERT_NAME:
      case PARSER_ALERT_CORRELATIONALERT_NAME:
      case PARSER_HEARTBEAT_ANALYZER_NODE_NAME:
      case PARSER_HEARTBEAT_ANALYZER_PROCESS_NAME:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_NODE_NAME:
      case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_NAME:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_NAME:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_NAME:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_NODE_NAME:
      case PARSER_ASSET_ANALYZER_PROCESS_NAME:
/*      case PARSER_ASSET_ANALYZER_ANALYZER_NODE_NAME:
      case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_NAME:*/
      case PARSER_ASSET_TARGET_NODE_NAME:
      case PARSER_ASSET_TARGET_PROCESS_NAME:
      case PARSER_ASSET_TARGET_USER_USERID_NAME:
      case PARSER_ASSET_TARGET_SERVICE_NAME:
      case PARSER_ASSET_TARGET_FILE_NAME:
      case PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NAME:
      case PARSER_ASSET_TARGET_FILE_LINKAGE_NAME:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_NAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_NAME:
      case PARSER_ASSET_TOOL_RESULT_REFERENCE_NAME:
#endif      
	idmef_parser_warning(state, "<name> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_NODE_LOCATION:
      case PARSER_ALERT_SOURCE_NODE_LOCATION:
      case PARSER_ALERT_TARGET_NODE_LOCATION:
      case PARSER_HEARTBEAT_ANALYZER_NODE_LOCATION:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_NODE_LOCATION:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_LOCATION:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_NODE_LOCATION:
//      case PARSER_ASSET_ANALYZER_ANALYZER_NODE_LOCATION:
      case PARSER_ASSET_TARGET_NODE_LOCATION:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE_LOCATION:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_LOCATION:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_LOCATION:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_LOCATION:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_LOCATION:
#endif
	idmef_parser_warning(state, "<location> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_NODE_ADDRESS_NETMASK:
      case PARSER_ALERT_SOURCE_NODE_ADDRESS_NETMASK:
      case PARSER_ALERT_TARGET_NODE_ADDRESS_NETMASK:
      case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_NETMASK:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_NODE_ADDRESS_NETMASK:
//      case PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TARGET_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS_NETMASK:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS_NETMASK:
#endif
	idmef_parser_warning(state, "<netmask> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_NODE_ADDRESS_ADDRESS:
      case PARSER_ALERT_SOURCE_NODE_ADDRESS_ADDRESS:
      case PARSER_ALERT_TARGET_NODE_ADDRESS_ADDRESS:
      case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_ADDRESS:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_NODE_ADDRESS_ADDRESS:
//      case PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TARGET_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS_ADDRESS:
#endif
	idmef_parser_warning(state, "<address> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_PROCESS_PID:
      case PARSER_ALERT_SOURCE_PROCESS_PID:
      case PARSER_ALERT_TARGET_PROCESS_PID:
      case PARSER_HEARTBEAT_ANALYZER_PROCESS_PID:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PID:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PID:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_PROCESS_PID:
//      case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PID:
      case PARSER_ASSET_TARGET_PROCESS_PID:
#endif
	idmef_parser_warning(state, "<pid> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_PROCESS_PATH:
      case PARSER_ALERT_SOURCE_PROCESS_PATH:
      case PARSER_ALERT_TARGET_PROCESS_PATH:
      case PARSER_ALERT_TARGET_FILE_PATH:
      case PARSER_ALERT_TARGET_FILE_LINKAGE_PATH:
      case PARSER_HEARTBEAT_ANALYZER_PROCESS_PATH:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PATH:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PATH:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_PROCESS_PATH:
//      case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PATH:
      case PARSER_ASSET_TARGET_PROCESS_PATH:
      case PARSER_ASSET_TARGET_FILE_PATH:
      case PARSER_ASSET_TARGET_FILE_LINKAGE_PATH:
#endif
	idmef_parser_warning(state, "<path> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_PROCESS_ARG:
      case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_ARG:
      case PARSER_ALERT_SOURCE_PROCESS_ARG:
      case PARSER_ALERT_TARGET_PROCESS_ARG:
      case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_ARG:
      case PARSER_HEARTBEAT_ANALYZER_PROCESS_ARG:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ARG:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ARG:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_PROCESS_ARG:
//      case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ARG:
      case PARSER_ASSET_TARGET_PROCESS_ARG:
      case PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_ARG:
      case PARSER_ASSET_TOOL_ARG:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_ARG:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_ARG:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_ARG:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_ARG:
#endif
	idmef_parser_warning(state, "<arg> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZER_PROCESS_ENV:
      case PARSER_ALERT_SOURCE_PROCESS_ENV:
      case PARSER_ALERT_TARGET_PROCESS_ENV:
      case PARSER_HEARTBEAT_ANALYZER_PROCESS_ENV:
/*      case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ENV:
      case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ENV:
*/
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ANALYZER_PROCESS_ENV:
//      case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ENV:
      case PARSER_ASSET_TARGET_PROCESS_ENV:
#endif
	idmef_parser_warning(state, "<env> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_USER_USERID_NUMBER:
      case PARSER_ALERT_TARGET_USER_USERID_NUMBER:
      case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NUMBER:
      case PARSER_ALERT_TARGET_FILE_INODE_NUMBER:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_USER_USERID_NUMBER:
      case PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NUMBER:
      case PARSER_ASSET_TARGET_FILE_INODE_NUMBER:
      case PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID_NUMBER:
#endif
	idmef_parser_warning(state, "<number> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_OID:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_OID:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_OID:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_OID:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_OID:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_OID:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_OID:
#endif
	idmef_parser_warning(state, "<oid> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMUNITY:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMUNITY:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMUNITY:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_COMMUNITY:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_COMMUNITY:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_COMMUNITY:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_COMMUNITY:
#endif
	idmef_parser_warning(state, "<community> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_SECURITYNAME:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_SECURITYNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_SECURITYNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_SECURITYNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_SECURITYNAME:
#endif
	idmef_parser_warning(state, "<securityName> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTNAME:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_CONTEXTNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_CONTEXTNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_CONTEXTNAME:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_CONTEXTNAME:
#endif
	idmef_parser_warning(state, "<contextName> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
#endif
	idmef_parser_warning(state, "<contextEngineID> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ALERT_TOOLALERT_COMMAND:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE_COMMAND:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE_COMMAND:
#endif
	idmef_parser_warning(state, "<command> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_URL:
      case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_URL:
      case PARSER_ALERT_CLASSIFICATION_REFERENCE_URL:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_URL:
      case PARSER_ASSET_TOOL_URL:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_URL:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_URL:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_URL:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_URL:
      case PARSER_ASSET_TOOL_RESULT_REFERENCE_URL:
#endif
	idmef_parser_warning(state, "<url> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_CGI:
      case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_CGI:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_CGI:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_CGI:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_CGI:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_CGI:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_CGI:
#endif
	idmef_parser_warning(state, "<cgi> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_HTTP_METHOD:
      case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE_HTTP_METHOD:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE_HTTP_METHOD:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE_HTTP_METHOD:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE_HTTP_METHOD:
#endif
	idmef_parser_warning(state, "<http-method> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_PORT:
      case PARSER_ALERT_TARGET_SERVICE_PORT:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_PORT:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_PORT:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PORT:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PORT:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PORT:
#endif
	idmef_parser_warning(state, "<PORT> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_PORTLIST:
      case PARSER_ALERT_TARGET_SERVICE_PORTLIST:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_PORTLIST:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_PORTLIST:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PORTLIST:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PORTLIST:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PORTLIST:
#endif
	idmef_parser_warning(state, "<portlist> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_SOURCE_SERVICE_PROTOCOL:
      case PARSER_ALERT_TARGET_SERVICE_PROTOCOL:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_SERVICE_PROTOCOL:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_PROTOCOL:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_PROTOCOL:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_PROTOCOL:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_PROTOCOL:
#endif
	idmef_parser_warning(state, "<protocol> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_CREATE_TIME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_CREATE_TIME:
#endif
	idmef_parser_warning(state, "<create-time> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_MODIFY_TIME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_MODIFY_TIME:      
#endif
	idmef_parser_warning(state, "<modify-time> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_ACCESS_TIME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_ACCESS_TIME:
#endif
	idmef_parser_warning(state, "<access-time> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_DATA_SIZE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_DATA_SIZE:
#endif
	idmef_parser_warning(state, "<data-size> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_DISK_SIZE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_DISK_SIZE:
#endif
	idmef_parser_warning(state, "<disk-size> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_FILEACCESS_PERMISSION:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_FILEACCESS_PERMISSION:
#endif
	idmef_parser_warning(state, "<permission> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_INODE_CHANGE_TIME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_INODE_CHANGE_TIME:
#endif
	idmef_parser_warning(state, "<change-time> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_INODE_MAJOR_DEVICE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_INODE_MAJOR_DEVICE:
#endif
	idmef_parser_warning(state, "<major-device> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_INODE_MINOR_DEVICE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_INODE_MINOR_DEVICE:
#endif
	idmef_parser_warning(state, "<minor-device> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_INODE_C_MAJOR_DEVICE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_INODE_C_MAJOR_DEVICE:
#endif
	idmef_parser_warning(state, "<c-major-device> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_INODE_C_MINOR_DEVICE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_INODE_C_MINOR_DEVICE:
#endif
	idmef_parser_warning(state, "<c-minor-device> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_CHECKSUM_VALUE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_CHECKSUM_VALUE:
#endif
	idmef_parser_warning(state, "<value> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TARGET_FILE_CHECKSUM_KEY:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TARGET_FILE_CHECKSUM_KEY:
#endif
	idmef_parser_warning(state, "<key> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_TOOLALERT_ALERTIDENT:
      case PARSER_ALERT_CORRELATIONALERT_ALERTIDENT:
	idmef_parser_warning(state, "<alertident> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_OVERFLOWALERT_PROGRAM:
	idmef_parser_warning(state, "<program> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_OVERFLOWALERT_SIZE:
	idmef_parser_warning(state, "<size> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_OVERFLOWALERT_BUFFER:
	idmef_parser_warning(state, "<buffer> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_CREATETIME:
      case PARSER_HEARTBEAT_CREATETIME:
	idmef_parser_warning(state, "<CreateTime> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ANALYZERTIME:
      case PARSER_HEARTBEAT_ANALYZERTIME:
	idmef_parser_warning(state, "<AnalyzerTime> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_DETECTTIME:
	idmef_parser_warning(state, "<DestectTime> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ASSESSMENT_IMPACT:
	idmef_parser_warning(state, "<Impact> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ASSESSMENT_ACTION:
	idmef_parser_warning(state, "<Action> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ASSESSMENT_CONFIDENCE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_TOOL_RESULT_CONFIDENCE:
#endif
	idmef_parser_warning(state, "<Confidence> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_BOOLEAN:
      case PARSER_HEARTBEAT_ADDITIONALDATA_BOOLEAN:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_BOOLEAN:
#endif
	idmef_parser_warning(state, "<boolean> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_BYTE:
      case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_BYTE:
#endif
	idmef_parser_warning(state, "<byte> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_CHARACTER:
      case PARSER_HEARTBEAT_ADDITIONALDATA_CHARACTER:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_CHARACTER:
#endif
	idmef_parser_warning(state, "<character> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_DATE_TIME:
      case PARSER_HEARTBEAT_ADDITIONALDATA_DATE_TIME:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_DATE_TIME:
#endif
	idmef_parser_warning(state, "<date-time> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_INTEGER:
      case PARSER_HEARTBEAT_ADDITIONALDATA_INTEGER:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_INTEGER:
#endif
	idmef_parser_warning(state, "<integer> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_NTPSTAMP:
      case PARSER_HEARTBEAT_ADDITIONALDATA_NTPSTAMP:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_NTPSTAMP:
#endif
	idmef_parser_warning(state, "<ntpstamp> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_PORTLIST:
      case PARSER_HEARTBEAT_ADDITIONALDATA_PORTLIST:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_PORTLIST:
#endif
	idmef_parser_warning(state, "<portlist> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_REAL:
      case PARSER_HEARTBEAT_ADDITIONALDATA_REAL:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_REAL:
#endif
	idmef_parser_warning(state, "<real> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_STRING:
      case PARSER_HEARTBEAT_ADDITIONALDATA_STRING:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_STRING:
#endif
	idmef_parser_warning(state, "<string> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_BYTE_STRING:
      case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE_STRING:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_BYTE_STRING:
#endif
	idmef_parser_warning(state, "<byte-string> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_ALERT_ADDITIONALDATA_XMLTEXT:
      case PARSER_HEARTBEAT_ADDITIONALDATA_XMLTEXT:
#ifdef IDMEF_ASSET
      case PARSER_ASSET_ADDITIONALDATA_XMLTEXT:
#endif
	idmef_parser_warning(state, "<xmltext> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
      case PARSER_HEARTBEAT_HEARTBEATINTERVAL:
	idmef_parser_warning(state, "<HeartbeatInterval> should be empty.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;

#ifdef IDMEF_ASSET
      case PARSER_ASSET:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        
    	if (!strcmp ((char*)name, "Analyzer"))
         {
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer\n");
#endif
          state->state = PARSER_ASSET_ANALYZER;
          state->prevstate = PARSER_ASSET;
          state->analyzer_count++;
          asset->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!asset->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    asset->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the analyzer with the asset node */
            libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              asset->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              asset->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              asset->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              asset->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              asset->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              asset->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              asset->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              asset->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->analyzer->idmefp)
                  asset->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->analyzer->idmefp, tidmefp);
                 }

                /* combine the analyzer with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
           } /* for */ 
         }
        else if (!strcmp ((char*)name, "Target"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target\n");
#endif
          state->state = PARSER_ASSET_TARGET;
          state->prevstate = PARSER_ASSET;
	  asset->target = i_new0 (IDMEFtarget, 1);

          if (!asset->target)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    asset->target->idmefp =
	        newTarget (NULL);

            /* combine the target with the asset node */
            libidmef_list_add_element (asset->idmefp, asset->target->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
          
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      asset->target->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->target->idmefp)
                  asset->target->idmefp =
                    newTarget (newTarget_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->target->idmefp, tidmefp);
                 }

                /* combine the target with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->target->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "decoy"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "decoy = %s\n", attrs[i+1]);
#endif
	      asset->target->decoy =
                get_decoy ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->target->idmefp)
                  asset->target->idmefp =
                    newTarget (newTarget_decoy ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_decoy ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->target->idmefp, tidmefp);
                 }

                /* combine the target with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->target->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "interface"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "interface = %s\n", attrs[i+1]);
#endif
	      asset->target->interface = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->target->idmefp)
                  asset->target->idmefp =
                    newTarget (newTarget_interface ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTarget_interface ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->target->idmefp, tidmefp);
                 }

                /* combine the target with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->target->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Target>",
                        attrs[i], attrs[i+1]);
	   }
         }
         else if (!strcmp ((char*)name, "Tool"))
         {
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool\n");
#endif
          state->state = PARSER_ASSET_TOOL;
          state->prevstate = PARSER_ASSET;
          asset->ntools++;
          ptr = i_renew (IDMEFtool, asset->tools, asset->ntools);
          
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          asset->tools = ptr;
          asset->tools[asset->ntools-1] = i_new0 (IDMEFtool, 1);

          if (!asset->tools[asset->ntools-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    asset->tools[asset->ntools-1]->idmefp = newTool (NULL);

            /* combine the tool with the asset node */
            libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->tool_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              asset->tools[asset->ntools-1]->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!asset->tools[asset->ntools-1]->idmefp)
                  asset->tools[asset->ntools-1]->idmefp =
                    newTool (newTool_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newTool_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (asset->tools[asset->ntools-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, asset->tools[asset->ntools-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Tool>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
           } /* for */ 
         }
        else
         {
          idmef_parser_warning (state, "Expected <Analyzer||Target|Tool> got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
      case PARSER_ASSET_ANALYZER:
       {
        IDMEFanalyzer *analyzer = get_asset_analyzer (state, -1);
                
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE;
          state->prevstate = PARSER_ASSET_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS;
          state->prevstate = PARSER_ASSET_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newProcess (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Analyzer"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer\n");
#endif
//          state->state = PARSER_ASSET_ANALYZER_ANALYZER;
          state->state = PARSER_ASSET_ANALYZER;
          state->prevstate = PARSER_ASSET_ANALYZER;
          state->analyzer_count++;          
          state->prevstate = PARSER_ASSET_ANALYZER;
          analyzer->analyzer = i_new0 (IDMEFanalyzer, 1);

          if (!analyzer->analyzer)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->analyzer->idmefp = newAnalyzer (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "analyzerid"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "analyzerid = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzerid = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_analyzerid ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_analyzerid ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "manufacturer"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "manufacturer = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->manufacturer = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_manufacturer ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_manufacturer ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "model"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "model = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->model = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_model ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_model ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "class"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "class = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->analyzer_class = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_class ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_class ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ostype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ostype = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->ostype = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_ostype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_ostype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "osversion"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "osversion = %s\n", attrs[i+1]);
#endif
              analyzer->analyzer->osversion = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->analyzer->idmefp)
                  analyzer->analyzer->idmefp =
                    newAnalyzer (newAnalyzer_osversion ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAnalyzer_osversion ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->analyzer->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->analyzer->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Analyzer>",
                        attrs[i], attrs[i+1]);
           } 
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process|Analyzer>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_ANALYZER_NODE:
       {
        IDMEFanalyzer *analyzer = get_asset_analyzer (state, -1);
        IDMEFnode *node = analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node location\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_ASSET_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node name\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_ASSET_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp = newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node Address\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_ASSET_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       } 
    case PARSER_ASSET_ANALYZER_NODE_ADDRESS:
       {
        IDMEFanalyzer *analyzer = get_asset_analyzer (state, -1);
        IDMEFaddress *address = analyzer->node->addresses[analyzer->node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ASSET_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Node Address address\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ASSET_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_ANALYZER_PROCESS:
       {
        IDMEFanalyzer *analyzer = get_asset_analyzer (state, -1);
        IDMEFprocess *process = analyzer->process;

        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process name\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_ASSET_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process pid\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_ASSET_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process path\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_ASSET_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process arg\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_ASSET_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);
          
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Process env\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_ASSET_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#if 0
    case PARSER_ASSET_ANALYZER_ANALYZER:
       {
        IDMEFanalyzer *analyzer =
                state->msg->assets[state->msg->nassets-1]->analyzer->analyzer;
                
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER;
          analyzer->node = i_new0 (IDMEFnode, 1);

          if (!analyzer->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->node->idmefp = newNode (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              analyzer->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->node->idmefp)
                  analyzer->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->node->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER;
          analyzer->process = i_new0 (IDMEFprocess, 1);

          if (!analyzer->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    analyzer->process->idmefp = newProcess (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              analyzer->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!analyzer->process->idmefp)
                  analyzer->process->idmefp =
                    newNode (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (analyzer->process->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (analyzer->idmefp, analyzer->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|Process>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_ANALYZER_ANALYZER_NODE:
       {
        IDMEFnode *node =
           state->msg->assets[state->msg->nassets-1]->analyzer->analyzer->node;

        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node location\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE_LOCATION;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node name\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE_NAME;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node Address\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);
          
          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                 get_address_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num = 
                strtoull ((char*)attrs[i+1], NULL, 10);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS:
       {
        IDMEFnode *node =
          state->msg->assets[state->msg->nassets-1]->analyzer->analyzer->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node Address netmask\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Node Address address\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_ANALYZER_ANALYZER_PROCESS:
       {
        IDMEFprocess *process =
          state->msg->assets[state->msg->nassets-1]->analyzer->analyzer->process;
          
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process name\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_NAME;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process pid\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PID;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process path\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_PATH;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process arg\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ARG;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Analyzer Analyzer Process env\n");
#endif
          state->state = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS_ENV;
          state->prevstate = PARSER_ASSET_ANALYZER_ANALYZER_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
#endif /* if 0 */
    case PARSER_ASSET_TARGET:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        
        if (!strcmp ((char*)name, "Node"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE;
          state->prevstate = PARSER_ASSET_TARGET;
          target->node = i_new0 (IDMEFnode, 1);

          if (!target->node)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->node->idmefp = newNode (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
           
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->node->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->node->idmefp)
                  target->node->idmefp =
                    newNode (newNode_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->node->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
              {
#ifdef IDMEF_DEBUG
               fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              target->node->category = get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->node->idmefp)
                  target->node->idmefp =
                    newNode (newNode_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newNode_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->node->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->node->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Node>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "User"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target User\n");
#endif
          state->state = PARSER_ASSET_TARGET_USER;
          state->prevstate = PARSER_ASSET_TARGET;
          target->user = i_new0 (IDMEFuser, 1);

          if (!target->user)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->user->idmefp = newNode (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->user->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->user->idmefp)
                  target->user->idmefp =
                    newNode (newUser_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->user->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              target->user->category = get_user_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->user->idmefp)
                  target->user->idmefp =
                    newNode (newUser_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUser_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->user->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->user->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <User>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Process"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS;
          state->prevstate = PARSER_ASSET_TARGET;
          target->process = i_new0 (IDMEFprocess, 1);

          if (!target->process)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->process->idmefp = newProcess (NULL);

            /* combine the asset with the message node */
            libidmef_list_add_element (target->idmefp, target->process->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->process->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->process->idmefp)
                  target->process->idmefp =
                    newNode (newProcess_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newProcess_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->process->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->process->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Process>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "Service"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE;
          state->prevstate = PARSER_ASSET_TARGET;
          target->service = i_new0 (IDMEFservice, 1);

          if (!target->service)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            target->service->idmefp =
               newService (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              target->service->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ip_version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ip_version = %s\n", attrs[i+1]);
#endif
              target->service->ip_version = strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_ip_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ip_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_number"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_number = %s\n", attrs[i+1]);
#endif
              target->service->iana_protocol_number =
                strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_iana_protocol_number ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_number ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_name = %s\n", attrs[i+1]);
#endif
              target->service->iana_protocol_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->service->idmefp)
                  target->service->idmefp =
                    newService (newService_iana_protocol_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->service->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (target->idmefp, target->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Service>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "File"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE;
          state->prevstate = PARSER_ASSET_TARGET;
          target->nfiles++;
          ptr = i_renew (IDMEFfile*,target->files,
                                     target->nfiles);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          target->files = ptr;
	  target->files[target->nfiles-1] = i_new0 (IDMEFfile, 1);

          if (!target->files[target->nfiles-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    target->files[target->nfiles-1]->idmefp = newFile (NULL);

            libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->category =
                get_file_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "fstype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "fstype = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->fstype =
                get_file_fstype ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_fstype ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_fstype ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "file-type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "file-type = %s\n", attrs[i+1]);
#endif
	      target->files[target->nfiles-1]->file_type = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!target->files[target->nfiles-1]->idmefp)
                  target->files[target->nfiles-1]->idmefp =
                    newFile (newFile_file_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newFile_file_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (target->files[target->nfiles-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (target->idmefp, target->files[target->nfiles-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <File>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <Node|User|Process|Service|File>,"
                     " got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_NODE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFnode *node = asset->target->node;
        
        if (!strcmp ((char*)name, "location"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node location\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE_LOCATION;
          state->prevstate = PARSER_ASSET_TARGET_NODE;
          node->location = i_new0 (IDMEFlocation, 1);

          if (!node->location)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->location->idmefp =
               newNode_location (NULL);

            /* combine the location with the node node */
            libidmef_list_add_element (node->idmefp, node->location->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <location>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node name\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE_NAME;
          state->prevstate = PARSER_ASSET_TARGET_NODE;
          node->name = i_new0 (IDMEFname, 1);

          if (!node->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            node->name->idmefp =
               newNode_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (node->idmefp, node->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node Address\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE_ADDRESS;
          state->prevstate = PARSER_ASSET_TARGET_NODE;
          node->naddresses++;
          ptr = i_renew (IDMEFaddress*, node->addresses,
          			node->naddresses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          node->addresses = ptr;
	  node->addresses[node->naddresses-1] = i_new0 (IDMEFaddress, 1);

          if (!node->addresses[node->naddresses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    node->addresses[node->naddresses-1]->idmefp =
	        newAddress (NULL);
            /* combine the address with the node node */
            libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }
	  
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->category =
                get_node_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-name = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else if (!strcmp((char*)attrs[i], "vlan-num"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vlan-num = %s\n", attrs[i+1]);
#endif
	      node->addresses[node->naddresses-1]->vlan_num =
                strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!node->addresses[node->naddresses-1]->idmefp)
                  node->addresses[node->naddresses-1]->idmefp =
                    newAddress (newAddress_vlan_num ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAddress_vlan_num ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (node->addresses[node->naddresses-1]->idmefp, tidmefp);
                 }

                /* combine the address with the node node */
                libidmef_list_add_element (node->idmefp, node->addresses[node->naddresses-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Address>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <location|name|Address>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_NODE_ADDRESS:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFnode *node = asset->target->node;
        IDMEFaddress *address = node->addresses[node->naddresses-1];
        
        if (!strcmp ((char*)name, "netmask"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node Address netmask\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE_ADDRESS_NETMASK;
          state->prevstate = PARSER_ASSET_TARGET_NODE_ADDRESS;
          address->netmask = i_new0 (IDMEFnetmask, 1);

          if (!address->netmask)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->netmask->idmefp =
               newAddress_netmask (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (address->idmefp, address->netmask->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <netmask>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "address"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Node Address address\n");
#endif
          state->state = PARSER_ASSET_TARGET_NODE_ADDRESS_ADDRESS;
          state->prevstate = PARSER_ASSET_TARGET_NODE_ADDRESS;
          address->address = i_new0 (IDMEFaddressaddress, 1);

          if (!address->address)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            address->address->idmefp =
               newAddress_address (NULL);

            /* combine the address with the address node */
            libidmef_list_add_element (address->idmefp, address->address->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <address>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <address|netmask>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_PROCESS:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFprocess *process = target->process;

        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process name\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS_NAME;
          state->prevstate = PARSER_ASSET_TARGET_PROCESS;
          process->name = i_new0 (IDMEFname, 1);

          if (!process->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->name->idmefp =
               newProcess_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (process->idmefp, process->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "pid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process pid\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS_PID;
          state->prevstate = PARSER_ASSET_TARGET_PROCESS;
          process->pid = i_new0 (IDMEFpid, 1);

          if (!process->pid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->pid->idmefp =
               newProcess_pid (NULL);

            /* combine the pid with the node node */
            libidmef_list_add_element (process->idmefp, process->pid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <pid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process path\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS_PATH;
          state->prevstate = PARSER_ASSET_TARGET_PROCESS;
          process->path = i_new0 (IDMEFpath, 1);

          if (!process->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->path->idmefp =
               newProcess_path (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process arg\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS_ARG;
          state->prevstate = PARSER_ASSET_TARGET_PROCESS;
          process->nargs++;
          ptr = i_renew (IDMEFarg*, process->args, process->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->args = ptr;
          process->args[process->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!process->args[process->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->args[process->nargs-1]->idmefp =
               newProcess_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->args[process->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "env"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Process env\n");
#endif
          state->state = PARSER_ASSET_TARGET_PROCESS_ENV;
          state->prevstate = PARSER_ASSET_TARGET_PROCESS;
          process->nenvs++;
          ptr = i_renew (IDMEFenv*, process->envs, process->nenvs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          process->envs = ptr;
          process->envs[process->nenvs-1] = i_new0 (IDMEFenv, 1);

          if (!process->envs[process->nenvs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            process->envs[process->nenvs-1]->idmefp =
               newProcess_env (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (process->idmefp, process->envs[process->nenvs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <env>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|pid|path|args|env>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       } 
    case PARSER_ASSET_TARGET_USER:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFuser *user = asset->target->user;
        
        if (!strcmp ((char*)name, "UserId"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target User UserId\n");
#endif

          state->state = PARSER_ASSET_TARGET_USER_USERID;
          state->prevstate = PARSER_ASSET_TARGET_USER;
          user->nuserids++;
          ptr = i_renew (IDMEFuserid*, user->userids, user->nuserids);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          user->userids = ptr;
          user->userids[user->nuserids-1] = i_new0 (IDMEFuserid, 1);

          if (!user->userids[user->nuserids-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            user->userids[user->nuserids-1]->idmefp =
               newUserId (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->ident = strdup ((char*)attrs[i+1]);
              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->type =
                get_userid_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "tty"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "tty = %s\n", attrs[i+1]);
#endif
	      user->userids[user->nuserids-1]->tty = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!user->userids[user->nuserids-1]->idmefp)
                  user->userids[user->nuserids-1]->idmefp =
                    newUserId (newUserId_tty ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_tty ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (user->userids[user->nuserids-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (user->idmefp, user->userids[user->nuserids-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <UserId>",
                        attrs[i], attrs[i+1]);
           }
         }
        else
         {
          idmef_parser_warning (state, "Expected <UserId>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_USER_USERID:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFuser *user = asset->target->user;
        IDMEFuserid *userid = user->userids[user->nuserids-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target User Userid name\n");
#endif
          state->state = PARSER_ASSET_TARGET_USER_USERID_NAME;
          state->prevstate = PARSER_ASSET_TARGET_USER_USERID;
          userid->name = i_new0 (IDMEFname, 1);

          if (!userid->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->name->idmefp = newUserId_name (NULL);

            /* combine the name with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target User Userid number\n");
#endif
          state->state = PARSER_ASSET_TARGET_USER_USERID_NUMBER;
          state->prevstate = PARSER_ASSET_TARGET_USER_USERID;
          userid->number = i_new0 (IDMEFnumber, 1);

          if (!userid->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->number->idmefp = newUserId_number (NULL);

            /* combine the number with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|number>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_SERVICE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFservice *service = asset->target->service;

        if (!strcmp ((char*)name, "SNMPService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->snmpservice = i_new0 (IDMEFsnmpservice, 1);

          if (!service->snmpservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->snmpservice->idmefp =
               newSNMPService (NULL);

            /* combine the snmpservice with the node node */
            libidmef_list_add_element (service->idmefp, service->snmpservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <SNMPService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "WebService"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service WebService\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->webservice = i_new0 (IDMEFwebservice, 1);

          if (!service->webservice)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->webservice->idmefp =
               newWebService (NULL);

            /* combine the webservice with the node node */
            libidmef_list_add_element (service->idmefp, service->webservice->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <WebService>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service name\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_NAME;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->name = i_new0 (IDMEFname, 1);

          if (!service->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->name->idmefp =
               newService_name (NULL);

            /* combine the name with the node node */
            libidmef_list_add_element (service->idmefp, service->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "port"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service port\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_PORT;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->port = i_new0 (IDMEFport, 1);

          if (!service->port)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->port->idmefp =
               newService_port (NULL);

            /* combine the port with the node node */
            libidmef_list_add_element (service->idmefp, service->port->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <port>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "portlist"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service portlist\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_PORTLIST;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->portlist = i_new0 (IDMEFportlist, 1);

          if (!service->portlist)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->portlist->idmefp =
               newService_portlist (NULL);

            /* combine the portlist with the node node */
            libidmef_list_add_element (service->idmefp, service->portlist->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <portlist>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "protocol"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service protocol\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_PROTOCOL;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE;
          service->protocol = i_new0 (IDMEFprotocol, 1);

          if (!service->protocol)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            service->protocol->idmefp =
               newService_protocol (NULL);

            /* combine the protocol with the node node */
            libidmef_list_add_element (service->idmefp, service->protocol->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <protocol>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <SNMPService|WebService|name|port|portlist|"
          	     "protocol>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFsnmpservice *snmpservice =
          asset->target->service->snmpservice;

        if (!strcmp ((char*)name, "oid"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService oid\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_OID;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->oid = i_new0 (IDMEFoid, 1);

          if (!snmpservice->oid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->oid->idmefp =
               newSNMPService_oid (NULL);

            /* combine the oid with the node node */
            libidmef_list_add_element (snmpservice->idmefp, snmpservice->oid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <oid>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "community"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService community\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMUNITY;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->community = i_new0 (IDMEFcommunity, 1);

          if (!snmpservice->community)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->community->idmefp =
               newSNMPService_community (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->community->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <community>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "securityName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService securityName\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->securityName = i_new0 (IDMEFsecurityName, 1);

          if (!snmpservice->securityName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->securityName->idmefp =
               newSNMPService_securityName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->securityName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <securityName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextName"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService contextName\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->contextName = i_new0 (IDMEFcontextName, 1);

          if (!snmpservice->contextName)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextName->idmefp =
               newSNMPService_contextName (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextName->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextName>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "contextEngineID"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService contextEngineID\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->contextEngineID = i_new0 (IDMEFcontextEngineID, 1);

          if (!snmpservice->contextEngineID)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->contextEngineID->idmefp =
               newSNMPService_contextEngineID (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->contextEngineID->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <contextEngineID>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "command"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service SNMPService command\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE_COMMAND;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_SNMPSERVICE;
          snmpservice->command = i_new0 (IDMEFcommand, 1);

          if (!snmpservice->command)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            snmpservice->command->idmefp =
               newSNMPService_command (NULL);

            libidmef_list_add_element (snmpservice->idmefp, snmpservice->command->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <command>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <oid|community|securityName|contextName|"
          	     "contextEngineID|command>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_SERVICE_WEBSERVICE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFwebservice *webservice =
          asset->target->service->webservice;

        if (!strcmp ((char*)name, "url"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service WebService url\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_URL;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE;
          webservice->url = i_new0 (IDMEFurl, 1);

          if (!webservice->url)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->url->idmefp =
               newWebService_url (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->url->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <url>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "cgi"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service WebService cgi\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_CGI;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE;
          webservice->cgi = i_new0 (IDMEFcgi, 1);

          if (!webservice->cgi)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->cgi->idmefp =
               newWebService_cgi (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->cgi->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <cgi>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "http-method"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service WebService http-method\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE;
          webservice->http_method = i_new0 (IDMEFhttp_method, 1);

          if (!webservice->http_method)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->http_method->idmefp =
               newWebService_http_method (NULL);

            libidmef_list_add_element (webservice->idmefp, webservice->http_method->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <http-method>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target Service WebService arg\n");
#endif
          state->state = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE_ARG;
          state->prevstate = PARSER_ASSET_TARGET_SERVICE_WEBSERVICE;
          webservice->nargs++;
          ptr = i_renew (IDMEFarg*, webservice->args, webservice->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          webservice->args = ptr;
          webservice->args[webservice->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!webservice->args[webservice->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            webservice->args[webservice->nargs-1]->idmefp =
               newWebService_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (webservice->idmefp, webservice->args[webservice->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <url|cgi|http-method|arg>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFfile *file = target->files[target->nfiles-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File name\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_NAME;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->name = i_new0 (IDMEFname, 1);

          if (!file->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->name->idmefp = newFile_name (NULL);

            libidmef_list_add_element (file->idmefp, file->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File path\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_PATH;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->path = i_new0 (IDMEFpath, 1);

          if (!file->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->path->idmefp = newFile_path (NULL);

            libidmef_list_add_element (file->idmefp, file->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "create-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File create-time\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_CREATE_TIME;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->create_time = i_new0 (IDMEFcreate_time, 1);

          if (!file->create_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->create_time->idmefp = newFile_create_time (NULL);

            libidmef_list_add_element (file->idmefp, file->create_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <create-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "modify-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File modify-time\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_MODIFY_TIME;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->modify_time = i_new0 (IDMEFmodify_time, 1);

          if (!file->modify_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->modify_time->idmefp = newFile_modify_time (NULL);

            libidmef_list_add_element (file->idmefp, file->modify_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <modify-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "access-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File access-time\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_ACCESS_TIME;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->access_time = i_new0 (IDMEFaccess_time, 1);

          if (!file->access_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->access_time->idmefp = newFile_access_time (NULL);

            libidmef_list_add_element (file->idmefp, file->access_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[0] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <access-time>",
                        attrs[0], attrs[1]);
         }
        else if (!strcmp ((char*)name, "data-size"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File data-size\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_DATA_SIZE;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->data_size = i_new0 (IDMEFdata_size, 1);

          if (!file->data_size)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->data_size->idmefp = newFile_data_size (NULL);

            libidmef_list_add_element (file->idmefp, file->data_size->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <data-size>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "disk-size"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File disk-size\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_DISK_SIZE;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->disk_size = i_new0 (IDMEFdisk_size, 1);

          if (!file->disk_size)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->disk_size->idmefp = newFile_disk_size (NULL);

            libidmef_list_add_element (file->idmefp, file->disk_size->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <disk-size>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "FileAccess"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File FileAccess\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_FILEACCESS;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->nfileaccesses++;
          ptr = i_renew (IDMEFfileaccess*,
                                        file->fileaccesses,
                                        file->nfileaccesses);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->fileaccesses = ptr;
	  file->fileaccesses[file->nfileaccesses-1] =
            i_new0 (IDMEFfileaccess, 1);

          if (!file->fileaccesses[file->nfileaccesses-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->fileaccesses[file->nfileaccesses-1]->idmefp =
               newFileAccess (NULL);

            /* combine the fileaccess with the file node */
            libidmef_list_add_element (file->idmefp, file->fileaccesses[file->nfileaccesses-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <FileAccess>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Linkage"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Linkage\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_LINKAGE;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->nlinkages++;
          ptr = i_renew (IDMEFlinkage*,
                                    file->linkages, file->nlinkages);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->linkages = ptr;
	  file->linkages[file->nlinkages-1] = i_new0 (IDMEFlinkage, 1);

          if (!file->linkages[file->nlinkages-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            file->linkages[file->nlinkages-1]->idmefp =
               newLinkage (NULL);

            /* combine the linkage with the file node */
            libidmef_list_add_element (file->idmefp, file->linkages[file->nlinkages-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
	      file->linkages[file->nlinkages-1]->category =
                get_linkage_category ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!file->linkages[file->nlinkages-1]->idmefp)
                  file->linkages[file->nlinkages-1]->idmefp =
                    newLinkage (newLinkage_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newLinkage_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (file->linkages[file->nlinkages-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (file->idmefp, file->linkages[file->nlinkages-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Linkage>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Inode"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->inode = i_new0 (IDMEFinode, 1);

          if (!file->inode)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            file->inode->idmefp = newInode (NULL);

            libidmef_list_add_element (file->idmefp, file->inode->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <Inode>",
                        attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Checksum"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Checksum\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_CHECKSUM;
          state->prevstate = PARSER_ASSET_TARGET_FILE;
          file->nchecksums++;
          ptr = i_renew (IDMEFchecksum*,
                                     file->checksums, file->nchecksums);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          file->checksums = ptr;
	  file->checksums[file->nchecksums-1] = i_new0 (IDMEFchecksum, 1);

          if (!file->checksums[file->nchecksums-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    file->checksums[file->nchecksums-1]->idmefp = newChecksum (NULL);

            libidmef_list_add_element (file->idmefp, file->checksums[file->nchecksums-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "algorithm"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "algorithm = %s\n", attrs[i+1]);
#endif
	      file->checksums[file->nchecksums-1]->algorithm =
                get_checksum_algorithm ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!file->checksums[file->nchecksums-1]->idmefp)
                  file->checksums[file->nchecksums-1]->idmefp =
                    newChecksum (newChecksum_algorithm ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newChecksum_algorithm ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (file->checksums[file->nchecksums-1]->idmefp, tidmefp);
                 }

                libidmef_list_add_element (file->idmefp, file->checksums[file->nchecksums-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
	     }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Checksum>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|path|create-time|modify-time|access-time|"
                     "data-size|disk-size|FileAccess|Linkage|Inode|Checksum>,"
                     " got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE_FILEACCESS:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFfileaccess *fileaccess = file->fileaccesses[file->nfileaccesses-1];
        
        if (!strcmp ((char*)name, "UserId"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File FileAccess UserId\n");
#endif

          state->state = PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID;
          state->prevstate = PARSER_ASSET_TARGET_FILE_FILEACCESS;
          fileaccess->userid = i_new0 (IDMEFuserid, 1);

          if (!fileaccess->userid)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            fileaccess->userid->idmefp = newUserId (NULL);

            /* combine the userid with the fileaccess node */
            libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              fileaccess->userid->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the userid with the fileaccess node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
              fileaccess->userid->type = get_userid_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the userid with the fileaccess node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "tty"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "tty = %s\n", attrs[i+1]);
#endif
	      fileaccess->userid->tty = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!fileaccess->userid->idmefp)
                  fileaccess->userid->idmefp =
                    newUserId (newUserId_tty ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newUserId_tty ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (fileaccess->userid->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (fileaccess->idmefp, fileaccess->userid->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <UserId>",
                        attrs[i], attrs[i+1]);
           }
         }
        else if (!strcmp ((char*)name, "permission"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File FileAccess permission\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_FILEACCESS_PERMISSION;
          state->prevstate = PARSER_ASSET_TARGET_FILE_FILEACCESS;
          fileaccess->npermissions++;
          ptr = i_renew (IDMEFpermission*, fileaccess->permissions,
                                             fileaccess->npermissions);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          fileaccess->permissions = ptr;
          fileaccess->permissions[fileaccess->npermissions-1] =
            i_new0 (IDMEFpermission, 1);
          
          if (!fileaccess->permissions[fileaccess->npermissions-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            fileaccess->permissions[fileaccess->npermissions-1]->idmefp =
              newFileAccess_permission (NULL);

            /* combine the  with the  node */
            libidmef_list_add_element (fileaccess->idmefp, fileaccess->permissions[fileaccess->npermissions-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <permission>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <UserId|permission>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFuserid *userid = file->fileaccesses[file->nfileaccesses-1]->userid;
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File FileAccess Userid name\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NAME;
          state->prevstate = PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID;
          userid->name = i_new0 (IDMEFname, 1);

          if (!userid->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->name->idmefp = newUserId_name (NULL);

            /* combine the name with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File FileAccess Userid number\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID_NUMBER;
          state->prevstate = PARSER_ASSET_TARGET_FILE_FILEACCESS_USERID;
          userid->number = i_new0 (IDMEFnumber, 1);

          if (!userid->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            userid->number->idmefp = newUserId_number (NULL);

            /* combine the number with the userid node */
            libidmef_list_add_element (userid->idmefp, userid->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|number>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE_LINKAGE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFlinkage *linkage = file->linkages[file->nlinkages-1];
        
        if (!strcmp ((char*)name, "name"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Linkage name\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_LINKAGE_NAME;
          state->prevstate = PARSER_ASSET_TARGET_FILE_LINKAGE;
          linkage->name = i_new0 (IDMEFname, 1);

          if (!linkage->name)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            linkage->name->idmefp = newLinkage_name (NULL);

            /* combine the name with the linkage node */
            libidmef_list_add_element (linkage->idmefp, linkage->name->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <name>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "path"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Linkage path\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_LINKAGE_PATH;
          state->prevstate = PARSER_ASSET_TARGET_FILE_LINKAGE;
          linkage->path = i_new0 (IDMEFpath, 1);

          if (!linkage->path)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            linkage->path->idmefp = newLinkage_path (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (linkage->idmefp, linkage->path->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <path>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <name|path>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE_INODE:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFinode *inode = target->files[target->nfiles-1]->inode;

        if (!strcmp ((char*)name, "change-time"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode change-time\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_CHANGE_TIME;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->change_time = i_new0 (IDMEFchange_time, 1);

          if (!inode->change_time)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->change_time->idmefp = newInode_change_time (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->change_time->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <change-time>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "number"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode number\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_NUMBER;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->number = i_new0 (IDMEFnumber, 1);

          if (!inode->number)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->number->idmefp = newInode_number (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->number->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <number>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "major-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode major-device\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_MAJOR_DEVICE;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->major_device = i_new0 (IDMEFmajor_device, 1);

          if (!inode->major_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->major_device->idmefp = newInode_major_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->major_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <major-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "minor-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode minor-device\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_MINOR_DEVICE;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->minor_device = i_new0 (IDMEFminor_device, 1);

          if (!inode->minor_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->minor_device->idmefp = newInode_minor_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->minor_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <minor-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "c-major-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode c-major-device\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_C_MAJOR_DEVICE;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->c_major_device = i_new0 (IDMEFc_major_device, 1);

          if (!inode->c_major_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->c_major_device->idmefp = newInode_c_major_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->c_major_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <c-major-device>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "c-minor-device"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Inode c-minor-device\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_INODE_C_MINOR_DEVICE;
          state->prevstate = PARSER_ASSET_TARGET_FILE_INODE;
          inode->c_minor_device = i_new0 (IDMEFc_minor_device, 1);

          if (!inode->c_minor_device)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            inode->c_minor_device->idmefp = newInode_c_minor_device (NULL);

            /* combine the path with the linkage node */
            libidmef_list_add_element (inode->idmefp, inode->c_minor_device->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <c-minor-device>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <change-time|number|major-device|minor-device|"
                     "c-major-device|c-minor-device>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
    case PARSER_ASSET_TARGET_FILE_CHECKSUM:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtarget *target = asset->target;
        IDMEFfile *file = target->files[target->nfiles-1];
        IDMEFchecksum *checksum = file->checksums[file->nchecksums-1];

        if (!strcmp ((char*)name, "value"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Checksum value\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_CHECKSUM_VALUE;
          state->prevstate = PARSER_ASSET_TARGET_FILE_CHECKSUM;
          checksum->value = i_new0 (IDMEFvalue, 1);

          if (!checksum->value)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            checksum->value->idmefp =
               newChecksum_value (NULL);

            libidmef_list_add_element (checksum->idmefp, checksum->value->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <value>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "key"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Target File Checksum key\n");
#endif
          state->state = PARSER_ASSET_TARGET_FILE_CHECKSUM_KEY;
          state->prevstate = PARSER_ASSET_TARGET_FILE_CHECKSUM;
          checksum->key = i_new0 (IDMEFkey, 1);

          if (!checksum->key)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            checksum->key->idmefp =
               newChecksum_key (NULL);

            libidmef_list_add_element (checksum->idmefp, checksum->key->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

	  if (attrs && attrs[i] != NULL)
	    idmef_parser_warning(state, "unknown attribute `%s=%s' for <key>",
                      attrs[i], attrs[i+1]);
         }
        else
         {
          idmef_parser_warning (state, "Expected <value|key>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
      case PARSER_ASSET_TOOL:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFtool *tool = asset->tools[asset->ntools-1];
        
    	if (!strcmp ((char*)name, "url"))
         {
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool url\n");
#endif
          state->state = PARSER_ASSET_TOOL_URL;
          state->prevstate = PARSER_ASSET_TOOL;
          tool->url = i_new0 (IDMEFurl, 1);

          if (!tool->url)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            tool->url->idmefp =
               newTool_url (NULL);

            libidmef_list_add_element (tool->idmefp, tool->url->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <url>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "arg"))
         {
          state->state = PARSER_ASSET_TOOL_ARG;
          state->prevstate = PARSER_ASSET_TOOL;
          tool->nargs++;
          ptr = i_renew (IDMEFarg*, tool->args, tool->nargs);

          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          tool->args = ptr;
          tool->args[tool->nargs-1] = i_new0 (IDMEFarg, 1);

          if (!tool->args[tool->nargs-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            tool->args[tool->nargs-1]->idmefp =
               newTool_arg (NULL);

            /* combine the path with the node node */
            libidmef_list_add_element (tool->idmefp, tool->args[tool->nargs-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          if (attrs && attrs[i] != NULL)
            idmef_parser_warning(state, "unknown attribute `%s=%s' for <arg>",
                      attrs[i], attrs[i+1]);
         }
        else if (!strcmp ((char*)name, "Result"))
         {
          
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool Result\n");
#endif
          state->state = PARSER_ASSET_TOOL_RESULT;
          state->prevstate = PARSER_ASSET_TOOL;
          tool->nresults++;
          ptr = i_renew (IDMEFtool, tool->results, tool->nresults);
          
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          tool->results = ptr;
          tool->results[tool->nresults-1] = i_new0 (IDMEFresult, 1);

          if (!tool->results[asset->ntools-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    tool->results[tool->nresults-1]->idmefp = newTool (NULL);

            /* combine the tool with the asset node */
            libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              tool->results[tool->nresults-1]->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->results[tool->nresults-1]->idmefp)
                  tool->results[tool->nresults-1]->idmefp =
                    newResult (newResult_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp = newResult_ident  ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->results[tool->nresults-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "severity"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "severity = %s\n", attrs[i+1]);
#endif
              tool->results[tool->nresults-1]->severity = get_result_severity ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->results[tool->nresults-1]->idmefp)
                  tool->results[tool->nresults-1]->idmefp =
                    newResult (newResult_severity ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newResult_severity ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->results[tool->nresults-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              tool->results[tool->nresults-1]->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->results[tool->nresults-1]->idmefp)
                  tool->results[tool->nresults-1]->idmefp =
                    newResult (newResult_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newResult_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->results[tool->nresults-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "category"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "category = %s\n", attrs[i+1]);
#endif
              tool->results[tool->nresults-1]->category = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->results[tool->nresults-1]->idmefp)
                  tool->results[tool->nresults-1]->idmefp =
                    newResult (newResult_category ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newResult_category ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->results[tool->nresults-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
              tool->results[tool->nresults-1]->type = get_result_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->results[tool->nresults-1]->idmefp)
                  tool->results[tool->nresults-1]->idmefp =
                    newResult (newResult_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newResult_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->results[tool->nresults-1]->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (asset->idmefp, tool->results[tool->nresults-1]->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Result>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
           } /* for */ 
         }
        else if (!strcmp ((char*)name, "AdditionalData"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool AdditionalData\n");
#endif
          state->state = PARSER_ASSET_TOOL_ADDITIONALDATA;
          state->prevstate = PARSER_ASSET_TOOL;
          tool->nadditionaldatas++;
          ptr = i_renew (IDMEFadditionaldata*,
                                     tool->additionaldatas,
          			     tool->nadditionaldatas);
          if (!ptr)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          tool->additionaldatas = ptr;
          tool->additionaldatas[tool->nadditionaldatas-1] =
            i_new0 (IDMEFadditionaldata, 1);
          
          if (!tool->additionaldatas[tool->nadditionaldatas-1])
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            tool->additionaldatas[tool->nadditionaldatas-1]->idmefp =
               newAdditionalData (NULL);

            /* combine the additional data with the asset node */
            libidmef_list_add_element (tool->idmefp, tool->additionaldatas[tool->nadditionaldatas-1]->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "type"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "type = %s\n", attrs[i+1]);
#endif
	      tool->additionaldatas[tool->nadditionaldatas-1]->type =
                 get_additionaldata_type ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->additionaldatas[tool->nadditionaldatas-1]->idmefp)
                  tool->additionaldatas[tool->nadditionaldatas-1]->idmefp =
                    newAdditionalData ("", newAdditionalData_type ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newAdditionalData_type ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (tool->additionaldatas[tool->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    tool->additionaldatas[tool->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "meaning"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "meaning = %s\n", attrs[i+1]);
#endif
	      tool->additionaldatas[tool->nadditionaldatas-1]->meaning =
                 strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!tool->additionaldatas[tool->nadditionaldatas-1]->idmefp)
                  tool->additionaldatas[tool->nadditionaldatas-1]->idmefp =
                    newAdditionalData ("", newAdditionalData_meaning ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newAdditionalData_meaning ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (tool->additionaldatas[tool->nadditionaldatas-1]->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    tool->additionaldatas[tool->nadditionaldatas-1]->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <AdditionalData>",
                        attrs[i], attrs[i+1]);
           } /* for */
         }
        else
         {
          idmef_parser_warning (state, "Expected <url|arg|Result|Additionaldata>, got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         }
         
        break;
       }

      case PARSER_ASSET_TOOL_RESULT:
       {
        IDMEFasset *asset = state->msg->assets[state->msg->nassets-1];
        IDMEFresult * result =
          asset->tools[asset->ntools-1]->results[asset->tools[asset->ntools-1]->nresults-1];
        
if (!strcmp ((char*)name, "Confidence"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool Result Confidence\n");
#endif
          state->state = PARSER_ASSET_TOOL_RESULT_CONFIDENCE;
          state->prevstate = PARSER_ASSET_TOOL_RESULT;
          result->confidence = i_new0 (IDMEFconfidence, 1);

          if (!result->confidence)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    result->confidence->idmefp = newConfidence (NULL);

            libidmef_list_add_element (result->idmefp, result->confidence->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

	  for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
	    if (!strcmp((char*)attrs[i], "rating"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "rating = %s\n", attrs[i+1]);
#endif
	      result->confidence->rating =
                get_confidence_rating ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->confidence->idmefp)
                  result->confidence->idmefp =
                    newConfidence ("", newConfidence_rating ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newConfidence_rating ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->confidence->idmefp, tidmefp);
                 }

                libidmef_list_add_element (result->idmefp, result->confidence->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
	      idmef_parser_warning(state, "unknown attribute `%s=%s' for <Confidence>",
                        attrs[i], attrs[i+1]);
	   }
         }
        else if (!strcmp ((char*)name, "Service"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool Result Service\n");
#endif
          state->state = PARSER_ASSET_TOOL_RESULT_SERVICE;
          state->prevstate = PARSER_ASSET_TOOL_RESULT;
          result->service = i_new0 (IDMEFservice, 1);

          if (!result->service)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

          if (state->gen_idmefp)
           {
            result->service->idmefp =
               newService (NULL);

            /* combine the netmask with the address node */
            libidmef_list_add_element (result->idmefp, result->service->idmefp);

#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
              result->service->ident = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->service->idmefp)
                  result->service->idmefp =
                    newService (newService_ident ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newService_ident ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (result->idmefp, result->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "ip_version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ip_version = %s\n", attrs[i+1]);
#endif
              result->service->ip_version = strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!result->service->idmefp)
                  result->service->idmefp =
                    newService (newService_ip_version ((char*)attrs[i+1]), NULL);
                else
                 {
                  tidmefp =  newService_ip_version ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (result->idmefp, result->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_number"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_number = %s\n", attrs[i+1]);
#endif
              result->service->iana_protocol_number =
                    strtoull ((char*)attrs[i+1], NULL, 0);

              if (state->gen_idmefp)
               {
                if (!result->service->idmefp)
                  result->service->idmefp =
                    newService (newService_iana_protocol_number ((char*)attrs[i+1]), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_number ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (result->idmefp, result->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "iana_protocol_name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "iana_protocol_name = %s\n", attrs[i+1]);
#endif
              result->service->iana_protocol_name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->service->idmefp)
                  result->service->idmefp =
                    newService (newService_iana_protocol_name ((char*)attrs[i+1]), NULL);
                else
                 {
                  tidmefp =  newService_iana_protocol_name ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->service->idmefp, tidmefp);
                 }

                /* combine the alert with the message node */
                libidmef_list_add_element (result->idmefp, result->service->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Service>",
                        attrs[i], attrs[i+1]);
           }
         }
       	else if (!strcmp ((char*)name, "Vulnerability"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool Result Vulnerability\n");
#endif
          state->state = PARSER_ASSET_TOOL_RESULT_VULNERABILITY;
          state->prevstate = PARSER_ASSET_TOOL_RESULT;
          result->vulnerability = i_new0 (IDMEFvulnerability, 1);

          if (!result->vulnerability)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
          if (state->gen_idmefp)
           {
            result->vulnerability->idmefp = newVulnerability (NULL);

            /* combine the additional data with the asset node */
            libidmef_list_add_element (result->idmefp, result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
            xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
           }
               
          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "ident"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "ident = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->ident = strdup((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_ident ((char*)attrs[i+1]), NULL);
                else
                 {
                  tidmefp =  newVulnerability_ident ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "exploitable"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "exploitable = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->exploitable =
              get_vulnerability_exploitable ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_exploitable ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_exploitable ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "location"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "location = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->location =
              get_vulnerability_location ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_location ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_location ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "attacktype"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "attacktype = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->attacktype =
              get_vulnerability_attacktype ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_attacktype ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_attacktype ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "attackimpact"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "attackimpact = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->attackimpact =
              get_vulnerability_attackimpact ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_attackimpact ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_attackimpact ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "grade"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "grade = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->grade =
              get_vulnerability_grade ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_grade ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_grade ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "web"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "web = %s\n", attrs[i+1]);
#endif
	      result->vulnerability->web =
              get_vulnerability_web ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->vulnerability->idmefp)
                  result->vulnerability->idmefp =
                    newVulnerability (newVulnerability_web ((char*)attrs[i+1]),
                                       NULL);
                else
                 {
                  tidmefp =  newVulnerability_web ((char*)attrs[i+1]);
                  libidmef_list_set_attribute (result->vulnerability->idmefp, tidmefp);
                 }

                /* combine the asset with the message node */
                libidmef_list_add_element (state->msg->idmefp,
                    result->vulnerability->idmefp);
#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <Vulnerability>",
                        attrs[i], attrs[i+1]);
           } /* for */
         }
        else if (!strcmp ((char*)name, "OS"))
         {
#ifdef IDMEF_DEBUG
          fprintf (stdout, "new state Asset Tool Result OS\n");
#endif
          state->state = PARSER_ASSET_TOOL_RESULT_OS;
          state->prevstate = PARSER_ASSET_TOOL_RESULT;
          result->os = i_new0 (IDMEFos, 1);

          if (!result->os)
            return idmef_parser_fatal_error (state, ENOMEM, "Not enough memory!");

	  /* create an idmefp element */
	  if (state->gen_idmefp)
	   {
	    result->os->idmefp = newOS (NULL);

            /* combine the result with the tool node */
            libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
	   }

          for (i = 0; attrs && attrs[i] != NULL; i += 2)
           {
            if (!strcmp((char*)attrs[i], "arch"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "arch = %s\n", attrs[i+1]);
#endif
              result->os->arch = get_os_arch ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->os->idmefp)
                  result->os->idmefp =
                    newOS (newOS_arch ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp = newOS_arch  ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->os->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "vendor"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "vendor = %s\n", attrs[i+1]);
#endif
              result->os->vendor = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->os->idmefp)
                  result->os->idmefp =
                    newOS (newOS_vendor ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newOS_vendor ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->os->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "family"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "family = %s\n", attrs[i+1]);
#endif
              result->os->family = get_os_family ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->os->idmefp)
                  result->os->idmefp =
                    newOS (newOS_family ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newOS_family ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->os->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "version"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "version = %s\n", attrs[i+1]);
#endif
              result->os->version = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->os->idmefp)
                  result->os->idmefp =
                    newOS (newOS_version ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newOS_version ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->os->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else if (!strcmp((char*)attrs[i], "name"))
             {
#ifdef IDMEF_DEBUG
              fprintf (stdout, "name = %s\n", attrs[i+1]);
#endif
              result->os->name = strdup ((char*)attrs[i+1]);

              if (state->gen_idmefp)
               {
                if (!result->os->idmefp)
                  result->os->idmefp =
                    newOS (newOS_name ((char*)attrs[i+1], NULL), NULL);
                else
                 {
                  tidmefp =  newOS_name ((char*)attrs[i+1], NULL);
                  libidmef_list_set_attribute (result->os->idmefp, tidmefp);
                 }

                /* combine the tool with the asset node */
                libidmef_list_add_element (result->idmefp, result->os->idmefp);

#ifdef IDMEF_DEBUG
                xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
               }
             }
            else
              idmef_parser_warning(state, "unknown attribute `%s=%s' for <OS>",
                        attrs[i], attrs[i+1]);

            if (tidmefp)
             {
//              tidmefp->IDMEFXmlNodePtr = NULL;
//              libidmef_idmefp_destroy (tidmefp);
              tidmefp = NULL;
             }
           } /* for */ 
         }
        else ??? weiter mit Rule
         {
          idmef_parser_warning (state, "Expected "
            "<Confidence|Service|Vulnerability|OS|Rule|Reference|CreateTime|banner|summary|description>"
            "got <%s>.", name);
          state->prevstate = state->state;
          state->state = PARSER_UNKNOWN;
          state->unknown_depth++;
         } 
        break;
       }
       
      case PARSER_ASSET_TOOL_RESULT_SERVICE:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_SNMPSERVICE:
      case PARSER_ASSET_TOOL_RESULT_SERVICE_WEBSERVICE:
      case PARSER_ASSET_TOOL_RESULT_VULNERABILITY:
      case PARSER_ASSET_TOOL_RESULT_VULNERABILITY_EXPLOIT:
      case PARSER_ASSET_TOOL_RESULT_OS:
      case PARSER_ASSET_TOOL_RESULT_OS_PATCHLEVEL:
      case PARSER_ASSET_TOOL_RESULT_OS_HOTFIX:
      case PARSER_ASSET_TOOL_RESULT_RULE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NODE_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_USER:
      case PARSER_ASSET_TOOL_RESULT_RULE_USER_USERID:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_SNMPSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_SERVICE_WEBSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGSRC_NODE_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_NODE_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_SNMPSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_ORGTGT_SERVICE_WEBSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSSRC_NODE_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_NODE_ADDRESS:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_SNMPSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_NAT_TRANSTGT_SERVICE_WEBSERVICE:
      case PARSER_ASSET_TOOL_RESULT_RULE_FWTYPE:
      case PARSER_ASSET_TOOL_RESULT_RULE_FWVERSION:
      case PARSER_ASSET_TOOL_RESULT_REFERENCE:
      case PARSER_ASSET_TOOL_RESULT_CREATETIME:
      case PARSER_ASSET_TOOL_RESULT_BANNER:
      case PARSER_ASSET_TOOL_RESULT_SUMMARY:
      case PARSER_ASSET_TOOL_RESULT_DESCRIPTION:
        break;
#endif
        
      case PARSER_UNKNOWN:
        break;
        
      case PARSER_END:
	idmef_parser_warning(state, "There should be no elements here.  Found <%s>.", name);
	state->prevstate = state->state;
	state->state = PARSER_UNKNOWN;        
        break;
        
   } /* switch state */
 }
 
static void idmef_parser_end_element (SAXParserState *state, const xmlChar *name)
 {
#ifdef IDMEF_DEBUG
  fprintf (stdout, "idmef_parser_end_element called for </%s>, state %d, "
             "prevstate = %d\n", name, state->state, state->prevstate);
#endif

  switch (state->state)
   {
    case PARSER_START:
      idmef_parser_warning (state, "should not be closing any elements in this state");
      break;

    case PARSER_UNKNOWN:
	state->unknown_depth--;
	if (state->unknown_depth == 0)
	    state->state = state->prevstate;
      break;

    case PARSER_IDMEF_MESSAGE:
      if (strcmp ((char*)name, "IDMEF-Message") != 0)
	idmef_parser_warning (state, "should find </IDMEF-Message> here.  Found </%s>",
		      name);
      state->state = PARSER_END;
      break;

    case PARSER_ALERT:
      if (strcmp ((char*)name, "Alert") != 0)
	idmef_parser_warning (state, "should find </Alert> here.  Found </%s>",
		      name);
      state->state = PARSER_IDMEF_MESSAGE;
      break;

    case PARSER_ALERT_ANALYZER:
     {
      if (strcmp ((char*)name, "Analyzer") != 0)
	idmef_parser_warning (state, "should find </Analyzer> here.  Found </%s>",
		      name);
                      
      /* analyzer path tracking */
      if (state->analyzer_count > 1)
        state->state = PARSER_ALERT_ANALYZER; /* stay in Analyzer */
      else
        state->state = PARSER_ALERT;

      state->analyzer_count--;
      break;
     }

    case PARSER_ALERT_ANALYZER_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER;
      break;

    case PARSER_ALERT_ANALYZER_NODE_NAME:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFnode *node = analyzer->node;

      state->state = PARSER_ALERT_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
      xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_NODE_LOCATION:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFnode *node = analyzer->node;

      state->state = PARSER_ALERT_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
        break;
       }
       
      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER_NODE;
      break;

    case PARSER_ALERT_ANALYZER_NODE_ADDRESS_NETMASK:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFaddress *address = analyzer->node->addresses[analyzer->node->naddresses-1];

      state->state = PARSER_ALERT_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
        break;
       }
       
      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_NODE_ADDRESS_ADDRESS:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFaddress *address = analyzer->node->addresses[analyzer->node->naddresses-1];

      state->state = PARSER_ALERT_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
        break;
       }
       
      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER;
      break;

    case PARSER_ALERT_ANALYZER_PROCESS_NAME:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_ALERT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_PROCESS_PID:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_ALERT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
        break;
       }
       
      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_PROCESS_PATH:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_ALERT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
        break;
       }
       
      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ANALYZER_PROCESS_ARG:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_ALERT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nargs++;
      process->args = i_renew (IDMEFarg*, process->args, process->nargs);*/
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ANALYZER_PROCESS_ENV:
     {
      IDMEFanalyzer *analyzer = get_alert_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_ALERT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "env") != 0)
       {
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nenvs++;
      process->envs = i_renew (IDMEFenv*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
#if 0
    case PARSER_ALERT_ANALYZER_ANALYZER:
      if (strcmp ((char*)name, "Analyzer") != 0)
	idmef_parser_warning (state, "should find </Analyzer> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER;
      break;

    case PARSER_ALERT_ANALYZER_ANALYZER_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER_ANALYZER;
      break;

    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_NAME:
     {
      IDMEFnode *node =
           state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;

      state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_LOCATION:
     {
      IDMEFnode *node =
           state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;

      state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
        break;
       }
       
      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE;
      break;

    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
     {
      IDMEFnode *node =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
        break;
       }
       
      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
     {
      IDMEFnode *node =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
        break;
       }
       
      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_ANALYZER_ANALYZER;
      break;

    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_NAME:
     {
      IDMEFprocess *process =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PID:
     {
      IDMEFprocess *process =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
        break;
       }
       
      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PATH:
     {
      IDMEFprocess *process =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
        break;
       }
       
      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ARG:
     {
      IDMEFprocess *process =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nargs++;
      process->args = i_renew (char*, process->args, process->nargs); */
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
      }
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ENV:
     {
      IDMEFprocess *process =
        state->msg->alerts[state->msg->nalerts-1]->analyzer->analyzer->process;
          
      state->state = PARSER_ALERT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "env") != 0)
       {
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nenvs++;
      process->envs = i_renew (char*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
#endif /* if 0*/
    case PARSER_ALERT_CREATETIME:
     {
      IDMEFtime *time = state->msg->alerts[state->msg->nalerts-1]->createtime;

      state->state = PARSER_ALERT;
      
      if (strcmp ((char*)name, "CreateTime") != 0)
       {
	idmef_parser_warning (state, "should find </CreateTime> here.  Found </%s>",
		      name);
        break;
       }
       
      time->string = strdup (state->content);
/*      time->tv = ??? */

      if (state->gen_idmefp)
       {
        if (!time->idmefp)
          idmef_parser_warning (state, "DTD violation: <CreateTime> missing "
          			       "ntpstamp attribute!");
        else
         {
          xmlNodeAddContent (time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
          xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
         }
       }
      break;
     }
    case PARSER_ALERT_ANALYZERTIME:
     {
      IDMEFtime *time = state->msg->alerts[state->msg->nalerts-1]->analyzertime;

      state->state = PARSER_ALERT;
      
      if (strcmp ((char*)name, "AnalyzerTime") != 0)
       {
	idmef_parser_warning (state, "should find </AnalyzerTime> here.  Found </%s>",
		      name);
        break;
       }
       
      time->string = strdup (state->content);
/*      time->tv = ??? */

      if (state->gen_idmefp)
       {
        if (!time->idmefp)
          idmef_parser_warning (state, "DTD violation: <AnalyzerTime> missing "
          			       "ntpstamp attribute!");
        else
         {
          xmlNodeAddContent (time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
          xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
         }
       }
      break;
     }
    case PARSER_ALERT_DETECTTIME:
     {
      IDMEFtime *time = state->msg->alerts[state->msg->nalerts-1]->detecttime;

      state->state = PARSER_ALERT;
      
      if (strcmp ((char*)name, "DetectTime") != 0)
       {
	idmef_parser_warning (state, "should find </DetectTime> here.  Found </%s>",
		      name);
        break;
       }
       
      time->string = strdup (state->content);
/*      time->tv = ??? */

      if (state->gen_idmefp)
       {
        if (!time->idmefp)
          idmef_parser_warning (state, "DTD violation: <DetectTime> missing "
          			       "ntpstamp attribute!");
        else
         {
          xmlNodeAddContent (time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
          xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
         }
       }
      break;
     }
    case PARSER_ALERT_SOURCE:
      if (strcmp ((char*)name, "Source") != 0)
	idmef_parser_warning (state, "should find </Source> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_SOURCE_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE;
      break;

    case PARSER_ALERT_SOURCE_NODE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->sources[alert->nsources-1]->node;

      state->state = PARSER_ALERT_SOURCE_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_NODE_LOCATION:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->sources[alert->nsources-1]->node;

      state->state = PARSER_ALERT_SOURCE_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
        break;
       }
       
      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE_NODE;
      break;

    case PARSER_ALERT_SOURCE_NODE_ADDRESS_NETMASK:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->sources[alert->nsources-1]->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_ALERT_SOURCE_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
        break;
       }
       
      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_NODE_ADDRESS_ADDRESS:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->sources[alert->nsources-1]->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_ALERT_SOURCE_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
        break;
       }
       
      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE;
      break;

    case PARSER_ALERT_SOURCE_PROCESS_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->sources[alert->nsources-1]->process;
          
      state->state = PARSER_ALERT_SOURCE_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_PROCESS_PID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->sources[alert->nsources-1]->process;
          
      state->state = PARSER_ALERT_SOURCE_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
        break;
       }
       
      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_PROCESS_PATH:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->sources[alert->nsources-1]->process;
          
      state->state = PARSER_ALERT_SOURCE_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
        break;
       }
       
      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_PROCESS_ARG:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->sources[alert->nsources-1]->process;
          
      state->state = PARSER_ALERT_SOURCE_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nargs++;
      process->args = i_renew (char*, process->args, process->nargs); */
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_PROCESS_ENV:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->sources[alert->nsources-1]->process;
          
      state->state = PARSER_ALERT_SOURCE_PROCESS;
      
      if (strcmp ((char*)name, "env") != 0)
       {
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
        break;
       }
       
/*      process->nenvs++;
      process->envs = i_renew (char*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_USER:
      if (strcmp ((char*)name, "User") != 0)
	idmef_parser_warning (state, "should find </User> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE;
      break;

    case PARSER_ALERT_SOURCE_USER_USERID:
      if (strcmp ((char*)name, "UserId") != 0)
	idmef_parser_warning (state, "should find </UserId> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE_USER;
      break;

    case PARSER_ALERT_SOURCE_USER_USERID_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFuser *user = alert->sources[alert->nsources-1]->user;
      IDMEFuserid *userid = user->userids[user->nuserids-1];
      
      state->state = PARSER_ALERT_SOURCE_USER_USERID;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      userid->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_USER_USERID_NUMBER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFuser *user = alert->sources[alert->nsources-1]->user;
      IDMEFuserid *userid = user->userids[user->nuserids-1];
      
      state->state = PARSER_ALERT_SOURCE_USER_USERID;
      
      if (strcmp ((char*)name, "number") != 0)
       {
	idmef_parser_warning (state, "should find </number> here.  Found </%s>",
		      name);
        break;
       }
       
      userid->number->number = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->number->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE:
      if (strcmp ((char*)name, "Service") != 0)
	idmef_parser_warning (state, "should find </Service> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE;
      break;

    case PARSER_ALERT_SOURCE_SERVICE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->sources[alert->nsources-1]->service;

      state->state = PARSER_ALERT_SOURCE_SERVICE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }
       
      service->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_PORT:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->sources[alert->nsources-1]->service;

      state->state = PARSER_ALERT_SOURCE_SERVICE;
      
      if (strcmp ((char*)name, "port") != 0)
       {
	idmef_parser_warning (state, "should find </port> here.  Found </%s>",
		      name);
       break;
      }

      service->port->port = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->port->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_PORTLIST:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->sources[alert->nsources-1]->service;

      state->state = PARSER_ALERT_SOURCE_SERVICE;
      
      if (strcmp ((char*)name, "portlist") != 0)
       {
	idmef_parser_warning (state, "should find </portlist> here.  Found </%s>",
		      name);
       break;
      }

      service->portlist->portlist = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->portlist->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_PROTOCOL:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->sources[alert->nsources-1]->service;

      state->state = PARSER_ALERT_SOURCE_SERVICE;
      
      if (strcmp ((char*)name, "protocol") != 0)
       {
	idmef_parser_warning (state, "should find </protocol> here.  Found </%s>",
		      name);
       break;
      }

      service->protocol->protocol = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->protocol->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE:
      if (strcmp ((char*)name, "SNMPService") != 0)
	idmef_parser_warning (state, "should find </SNMPService> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE_SERVICE;
      break;

    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_OID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "oid") != 0)
       {
	idmef_parser_warning (state, "should find </oid> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->oid->oid = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->oid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMUNITY:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "community") != 0)
       {
	idmef_parser_warning (state, "should find </community> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->community->community = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->community->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_SECURITYNAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "securityName") != 0)
       {
	idmef_parser_warning (state, "should find </securityName> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->securityName->securityName = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->securityName->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTNAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "contextName") != 0)
       {
	idmef_parser_warning (state, "should find </contextName> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->contextName->contextName = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->contextName->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "contextEngineID") != 0)
       {
	idmef_parser_warning (state, "should find </contextEngineID> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->contextEngineID->contextEngineID = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->contextEngineID->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMAND:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->sources[alert->nsources-1]->service->snmpservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "command") != 0)
       {
	idmef_parser_warning (state, "should find </command> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->command->command = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->command->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE:
      if (strcmp ((char*)name, "WebService") != 0)
	idmef_parser_warning (state, "should find </WebService> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_SOURCE_SERVICE;
      break;

    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_URL:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->sources[alert->nsources-1]->service->webservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "url") != 0)
       {
	idmef_parser_warning (state, "should find </url> here.  Found </%s>",
		      name);
       break;
      }

      webservice->url->url = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->url->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_CGI:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->sources[alert->nsources-1]->service->webservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "cgi") != 0)
       {
	idmef_parser_warning (state, "should find </cgi> here.  Found </%s>",
		      name);
       break;
      }

      webservice->cgi->cgi = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->cgi->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_HTTP_METHOD:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->sources[alert->nsources-1]->service->webservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "http-method") != 0)
       {
	idmef_parser_warning (state, "should find </http-method> here.  Found </%s>",
		      name);
       break;
      }

      webservice->http_method->http_method = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->http_method->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_ARG:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->sources[alert->nsources-1]->service->webservice;

      state->state = PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
       break;
      }

/*      webservice->nargs++;
      webservice->args = i_renew (char*, webservice->args, webservice->nargs);
      */
      webservice->args[webservice->nargs-1]->arg = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->args[webservice->nargs-1]->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET:
      if (strcmp ((char*)name, "Target") != 0)
	idmef_parser_warning (state, "should find </Target> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_TARGET_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET;
      break;

    case PARSER_ALERT_TARGET_NODE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->targets[alert->ntargets-1]->node;

      state->state = PARSER_ALERT_TARGET_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_NODE_LOCATION:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->targets[alert->ntargets-1]->node;

      state->state = PARSER_ALERT_TARGET_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
       break;
      }

      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_NODE;
      break;

    case PARSER_ALERT_TARGET_NODE_ADDRESS_NETMASK:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->targets[alert->ntargets-1]->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];

      state->state = PARSER_ALERT_TARGET_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
       break;
      }

      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_NODE_ADDRESS_ADDRESS:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFnode *node = alert->targets[alert->ntargets-1]->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];

      state->state = PARSER_ALERT_TARGET_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
       break;
      }

      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET;
      break;

    case PARSER_ALERT_TARGET_PROCESS_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->targets[alert->ntargets-1]->process;
          
      state->state = PARSER_ALERT_TARGET_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_PROCESS_PID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->targets[alert->ntargets-1]->process;
          
      state->state = PARSER_ALERT_TARGET_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
       break;
      }

      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_PROCESS_PATH:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->targets[alert->ntargets-1]->process;
          
      state->state = PARSER_ALERT_TARGET_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
       break;
      }

      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_PROCESS_ARG:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->targets[alert->ntargets-1]->process;
          
      state->state = PARSER_ALERT_TARGET_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
       break;
      }

/*      process->nargs++;
      process->args = i_renew (char*, process->args, process->nargs); */
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_PROCESS_ENV:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFprocess *process = alert->targets[alert->ntargets-1]->process;
          
      if (strcmp ((char*)name, "env") != 0)
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_PROCESS;
/*      process->nenvs++;
      process->envs = i_renew (char*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_USER:
      if (strcmp ((char*)name, "User") != 0)
	idmef_parser_warning (state, "should find </User> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET;
      break;
     
    case PARSER_ALERT_TARGET_USER_USERID:
      if (strcmp ((char*)name, "UserId") != 0)
	idmef_parser_warning (state, "should find </UserId> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_USER;
      break;

    case PARSER_ALERT_TARGET_USER_USERID_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFuser *user = alert->targets[alert->ntargets-1]->user;
      IDMEFuserid *userid = user->userids[user->nuserids-1];
      
      state->state = PARSER_ALERT_TARGET_USER_USERID;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      userid->name->name = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_USER_USERID_NUMBER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFuser *user = alert->targets[alert->ntargets-1]->user;
      IDMEFuserid *userid = user->userids[user->nuserids-1];
      
      state->state = PARSER_ALERT_TARGET_USER_USERID;
      
      if (strcmp ((char*)name, "number") != 0)
       {
	idmef_parser_warning (state, "should find </number> here.  Found </%s>",
		      name);
       break;
      }

      userid->number->number = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->number->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE:
      if (strcmp ((char*)name, "Service") != 0)
	idmef_parser_warning (state, "should find </Service> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET;
      break;

    case PARSER_ALERT_TARGET_SERVICE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->targets[alert->ntargets-1]->service;

      state->state = PARSER_ALERT_TARGET_SERVICE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      service->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_PORT:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->targets[alert->ntargets-1]->service;

      state->state = PARSER_ALERT_TARGET_SERVICE;
      
      if (strcmp ((char*)name, "port") != 0)
       {
	idmef_parser_warning (state, "should find </port> here.  Found </%s>",
		      name);
       break;
      }

      service->port->port = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->port->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_PORTLIST:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->targets[alert->ntargets-1]->service;

      state->state = PARSER_ALERT_TARGET_SERVICE;
      
      if (strcmp ((char*)name, "portlist") != 0)
       {
	idmef_parser_warning (state, "should find </portlist> here.  Found </%s>",
		      name);
       break;
      }

      service->portlist->portlist = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->portlist->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_PROTOCOL:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFservice *service = alert->targets[alert->ntargets-1]->service;

      if (strcmp ((char*)name, "protocol") != 0)
	idmef_parser_warning (state, "should find </protocol> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_SERVICE;
      service->protocol->protocol = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (service->protocol->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE:
      state->state = PARSER_ALERT_TARGET_SERVICE;

      if (strcmp ((char*)name, "SNMPService") != 0)
	idmef_parser_warning (state, "should find </SNMPService> here.  Found </%s>",
		      name);
      break;

    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_OID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "oid") != 0)
       {
	idmef_parser_warning (state, "should find </oid> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->oid->oid = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->oid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMUNITY:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "community") != 0)
       {
	idmef_parser_warning (state, "should find </community> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->community->community = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->community->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "securityName") != 0)
       {
	idmef_parser_warning (state, "should find </securityName> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->securityName->securityName = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->securityName->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "contextName") != 0)
       {
	idmef_parser_warning (state, "should find </contextName> here.  Found </%s>",
		      name);
       break;
      }

      snmpservice->contextName->contextName = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->contextName->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "contextEngineID") != 0)
       {
	idmef_parser_warning (state, "should find </contextEngineID> here.  Found </%s>",
		      name);
        break;
       }

      snmpservice->contextEngineID->contextEngineID = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->contextEngineID->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMAND:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFsnmpservice *snmpservice =
        alert->targets[alert->ntargets-1]->service->snmpservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE;
      
      if (strcmp ((char*)name, "command") != 0)
       {
	idmef_parser_warning (state, "should find </command> here.  Found </%s>",
		      name);
        break;
       }

      snmpservice->command->command = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (snmpservice->command->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE:
      if (strcmp ((char*)name, "WebService") != 0)
	idmef_parser_warning (state, "should find </WebService> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_SERVICE;
      break;

    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_URL:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->targets[alert->ntargets-1]->service->webservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "url") != 0)
       {
	idmef_parser_warning (state, "should find </url> here.  Found </%s>",
		      name);
       break;
      }

      webservice->url->url = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->url->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_CGI:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->targets[alert->ntargets-1]->service->webservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "cgi") != 0)
       {
	idmef_parser_warning (state, "should find </cgi> here.  Found </%s>",
		      name);
       break;
      }

      webservice->cgi->cgi = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->cgi->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->targets[alert->ntargets-1]->service->webservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "http-method") != 0)
       {
	idmef_parser_warning (state, "should find </http-method> here.  Found </%s>",
		      name);
       break;
      }

      webservice->http_method->http_method = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->http_method->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_ARG:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFwebservice *webservice =
        alert->targets[alert->ntargets-1]->service->webservice;

      state->state = PARSER_ALERT_TARGET_SERVICE_WEBSERVICE;
      
      if (strcmp ((char*)name, "cgi") != 0)
       {
	idmef_parser_warning (state, "should find </cgi> here.  Found </%s>",
		      name);
       break;
      }

/*      webservice->nargs++;
      webservice->args = i_renew (char*, webservice->args, webservice->nargs);*/
      webservice->args[webservice->nargs-1]->arg = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (webservice->args[webservice->nargs-1]->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_TARGET_FILE:
      if (strcmp ((char*)name, "File") != 0)
	idmef_parser_warning (state, "should find </File> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET;
      break;

    case PARSER_ALERT_TARGET_FILE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      file->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_PATH:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
       break;
      }

      file->path->path= strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_CREATE_TIME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "create-time") != 0)
       {
	idmef_parser_warning (state, "should find </create-time> here.  Found </%s>",
		      name);
       break;
      }

      file->create_time->create_time = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->create_time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_MODIFY_TIME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "modify-time") != 0)
       {
	idmef_parser_warning (state, "should find </modify-time> here.  Found </%s>",
		      name);
       break;
      }

      file->modify_time->modify_time = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->modify_time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_ACCESS_TIME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "access-time") != 0)
       {
	idmef_parser_warning (state, "should find </access-time> here.  Found </%s>",
		      name);
       break;
      }

      file->access_time->access_time = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->access_time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_DATA_SIZE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "data-size") != 0)
       {
	idmef_parser_warning (state, "should find </data-size> here.  Found </%s>",
		      name);
       break;
      }

      file->data_size->data_size = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->data_size->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_DISK_SIZE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
        
      state->state = PARSER_ALERT_TARGET_FILE;
      
      if (strcmp ((char*)name, "disk-size") != 0)
       {
	idmef_parser_warning (state, "should find </disk-size> here.  Found </%s>",
		      name);
       break;
      }

      file->disk_size->disk_size = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (file->disk_size->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_FILEACCESS:
      if (strcmp ((char*)name, "FileAccess") != 0)
	idmef_parser_warning (state, "should find </FileAccess> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_FILE;
      break;

    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID:
      if (strcmp ((char*)name, "UserId") != 0)
	idmef_parser_warning (state, "should find </UserId> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS;
      break;

    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFuserid *userid = file->fileaccesses[file->nfileaccesses-1]->userid;
        
      state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      userid->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NUMBER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFuserid *userid = file->fileaccesses[file->nfileaccesses-1]->userid;
        
      state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID;
      
      if (strcmp ((char*)name, "number") != 0)
       {
	idmef_parser_warning (state, "should find </number> here.  Found </%s>",
		      name);
       break;
      }

      userid->number->number = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (userid->number->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_PERMISSION:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFfileaccess *fileaccess = file->fileaccesses[file->nfileaccesses-1];
        
      state->state = PARSER_ALERT_TARGET_FILE_FILEACCESS;
      
      if (strcmp ((char*)name, "permission") != 0)
       {
	idmef_parser_warning (state, "should find </permission> here.  Found </%s>",
		      name);
       break;
      }

      fileaccess->permissions[fileaccess->npermissions-1]->permission =
        strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (fileaccess->permissions[fileaccess->npermissions-1]->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_LINKAGE:
      if (strcmp ((char*)name, "Linkage") != 0)
	idmef_parser_warning (state, "should find </Linkage> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_FILE;
      break;

    case PARSER_ALERT_TARGET_FILE_LINKAGE_NAME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFlinkage *linkage = file->linkages[file->nlinkages-1];

      state->state = PARSER_ALERT_TARGET_FILE_LINKAGE;
        
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      linkage->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (linkage->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_LINKAGE_PATH:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFlinkage *linkage = file->linkages[file->nlinkages-1];
        
      state->state = PARSER_ALERT_TARGET_FILE_LINKAGE;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
       break;
      }

      linkage->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (linkage->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE:
      if (strcmp ((char*)name, "Inode") != 0)
	idmef_parser_warning (state, "should find </Inode> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_FILE;
      break;

    case PARSER_ALERT_TARGET_FILE_INODE_CHANGE_TIME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "change-time") != 0)
       {
	idmef_parser_warning (state, "should find </change-time> here.  Found </%s>",
		      name);
       break;
      }

      inode->change_time->change_time = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->change_time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE_NUMBER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "number") != 0)
       {
	idmef_parser_warning (state, "should find </number> here.  Found </%s>",
		      name);
       break;
      }

      inode->number->number = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->number->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE_MAJOR_DEVICE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "major-device") != 0)
       {
	idmef_parser_warning (state, "should find </major-device> here.  Found </%s>",
		      name);
       break;
      }

      inode->major_device->major_device = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->major_device->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE_MINOR_DEVICE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "minor-device") != 0)
       {
	idmef_parser_warning (state, "should find </minor-device> here.  Found </%s>",
		      name);
       break;
      }

      inode->minor_device->minor_device = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->minor_device->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE_C_MAJOR_DEVICE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "c-major-device") != 0)
       {
	idmef_parser_warning (state, "should find </c-major-device> here.  Found </%s>",
		      name);
       break;
      }

      inode->c_major_device->c_major_device = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->c_major_device->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_INODE_C_MINOR_DEVICE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFinode *inode = target->files[target->nfiles-1]->inode;
      
      state->state = PARSER_ALERT_TARGET_FILE_INODE;
      
      if (strcmp ((char*)name, "c-minor-device") != 0)
       {
	idmef_parser_warning (state, "should find </c-minor-device> here.  Found </%s>",
		      name);
       break;
      }

      inode->c_minor_device->c_minor_device = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (inode->c_minor_device->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_CHECKSUM:
      if (strcmp ((char*)name, "Checksum") != 0)
	idmef_parser_warning (state, "should find </Checksum> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_TARGET_FILE;
      break;

    case PARSER_ALERT_TARGET_FILE_CHECKSUM_VALUE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFchecksum *checksum = file->checksums[file->nchecksums-1];
      
      state->state = PARSER_ALERT_TARGET_FILE_CHECKSUM;
      
      if (strcmp ((char*)name, "value") != 0)
       {
	idmef_parser_warning (state, "should find </value> here.  Found </%s>",
		      name);
       break;
      }

      checksum->value->value = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (checksum->value->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TARGET_FILE_CHECKSUM_KEY:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFtarget *target = alert->targets[alert->ntargets-1];
      IDMEFfile *file = target->files[target->nfiles-1];
      IDMEFchecksum *checksum = file->checksums[file->nchecksums-1];
      
      state->state = PARSER_ALERT_TARGET_FILE_CHECKSUM;
      
      if (strcmp ((char*)name, "key") != 0)
       {
	idmef_parser_warning (state, "should find </key> here.  Found </%s>",
		      name);
       break;
      }

      checksum->key->key = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (checksum->key->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_CLASSIFICATION:
      if (strcmp ((char*)name, "Classification") != 0)
	idmef_parser_warning (state, "should find </Classification> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_CLASSIFICATION_REFERENCE:
      if (strcmp ((char*)name, "Reference") != 0)
	idmef_parser_warning (state, "should find </Reference> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT_CLASSIFICATION;
      break;

    case PARSER_ALERT_CLASSIFICATION_REFERENCE_NAME:
     {
      IDMEFclassification *classification =
        state->msg->alerts[state->msg->nalerts-1]->classification;
      IDMEFreference *reference =
        classification->references[classification->nreferences-1];
        
      state->state = PARSER_ALERT_CLASSIFICATION_REFERENCE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      reference->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (reference->name->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_CLASSIFICATION_REFERENCE_URL:
     {
      IDMEFclassification *classification =
        state->msg->alerts[state->msg->nalerts-1]->classification;
      IDMEFreference *reference =
        classification->references[classification->nreferences-1];
        
      state->state = PARSER_ALERT_CLASSIFICATION_REFERENCE;
      
      if (strcmp ((char*)name, "url") != 0)
       {
	idmef_parser_warning (state, "should find </url> here.  Found </%s>",
		      name);
       break;
      }

      reference->url->url = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (reference->url->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ASSESSMENT:
      if (strcmp ((char*)name, "Assessment") != 0)
	idmef_parser_warning (state, "should find </Assessment> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_ASSESSMENT_IMPACT:
     {
      IDMEFimpact *impact =
        state->msg->alerts[state->msg->nalerts-1]->assessment->impact;
        
      state->state = PARSER_ALERT_ASSESSMENT;

      if (strcmp ((char*)name, "Impact") != 0)
	idmef_parser_warning (state, "should find </Impact> here.  Found </%s>",
		              name);
                      
      impact->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (impact->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ASSESSMENT_ACTION:
     {
      IDMEFassessment *assessment =
        state->msg->alerts[state->msg->nalerts-1]->assessment;
      IDMEFaction *action = assessment->actions[assessment->nactions-1];
        
      state->state = PARSER_ALERT_ASSESSMENT;
      
      if (strcmp ((char*)name, "Action") != 0)
       {
	idmef_parser_warning (state, "should find </Action> here.  Found </%s>",
		      name);
       break;
      }

      action->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (action->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ASSESSMENT_CONFIDENCE:
     {
      IDMEFconfidence *confidence =
        state->msg->alerts[state->msg->nalerts-1]->assessment->confidence;
        
      state->state = PARSER_ALERT_ASSESSMENT;
      
      if (strcmp ((char*)name, "Confidence") != 0)
       {
	idmef_parser_warning (state, "should find </Confidence> here.  Found </%s>",
		      name);
       break;
      }

      if (strlen (state->content) > 0)
        confidence->data = strtod(state->content, NULL);
      else
        confidence->data = 0.0;
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (confidence->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_ADDITIONALDATA:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT;
      
      if (strcmp ((char*)name, "AdditionalData") != 0)
       {
	idmef_parser_warning (state, "should find </AdditionalData> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_BOOLEAN:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "boolean") != 0)
       {
	idmef_parser_warning (state, "should find </boolean> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->boolean->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_BYTE:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "byte") != 0)
       {
	idmef_parser_warning (state, "should find </byte> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->byte->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_CHARACTER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "character") != 0)
       {
	idmef_parser_warning (state, "should find </character> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->character->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_DATE_TIME:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "date-time") != 0)
       {
	idmef_parser_warning (state, "should find </date-time> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->date_time->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_INTEGER:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "integer") != 0)
       {
	idmef_parser_warning (state, "should find </integer> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->integer->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_NTPSTAMP:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "ntpstamp") != 0)
       {
	idmef_parser_warning (state, "should find </ntpstamp> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->ntpstamp->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_PORTLIST:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "portlist") != 0)
       {
	idmef_parser_warning (state, "should find </portlist> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->portlist->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_REAL:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "real") != 0)
       {
	idmef_parser_warning (state, "should find </real> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->real->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_STRING:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "string") != 0)
       {
	idmef_parser_warning (state, "should find </string> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->string->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_BYTE_STRING:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "byte-string") != 0)
       {
	idmef_parser_warning (state, "should find </byte-string> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->byte_string->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_ADDITIONALDATA_XMLTEXT:
     {
      IDMEFalert *alert = state->msg->alerts[state->msg->nalerts-1];
      IDMEFadditionaldata *additionaldata =
        alert->additionaldatas[alert->nadditionaldatas-1];
      
      state->state = PARSER_ALERT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "xmltext") != 0)
       {
	idmef_parser_warning (state, "should find </xmltext> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->xmltext->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_ALERT_TOOLALERT:
      if (strcmp ((char*)name, "ToolAlert") != 0)
	idmef_parser_warning (state, "should find </ToolAlert> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_TOOLALERT_NAME:
     {
      IDMEFtoolalert *toolalert =
        state->msg->alerts[state->msg->nalerts-1]->toolalert;
        
      state->state = PARSER_ALERT_TOOLALERT;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
       break;
      }

      toolalert->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (toolalert->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TOOLALERT_COMMAND:
     {
      IDMEFtoolalert *toolalert =
        state->msg->alerts[state->msg->nalerts-1]->toolalert;
        
      state->state = PARSER_ALERT_TOOLALERT;
      
      if (strcmp ((char*)name, "command") != 0)
       {
	idmef_parser_warning (state, "should find </command> here.  Found </%s>",
		      name);
       break;
      }

      toolalert->command->command = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (toolalert->command->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_TOOLALERT_ALERTIDENT:
     {
      IDMEFtoolalert *toolalert =
        state->msg->alerts[state->msg->nalerts-1]->toolalert;
        
      state->state = PARSER_ALERT_TOOLALERT;
      
      if (strcmp ((char*)name, "alertident") != 0)
       {
	idmef_parser_warning (state, "should find </alertident> here.  Found </%s>",
		      name);
       break;
      }

      toolalert->alertidents[toolalert->nalertidents-1]->data =
        strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (toolalert->alertidents[toolalert->nalertidents-1]->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_OVERFLOWALERT:
      if (strcmp ((char*)name, "OverflowAlert") != 0)
	idmef_parser_warning (state, "should find </OverflowAlert> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_OVERFLOWALERT_PROGRAM:
     {
      IDMEFoverflowalert *overflowalert =
        state->msg->alerts[state->msg->nalerts-1]->overflowalert;
        
      state->state = PARSER_ALERT_OVERFLOWALERT;
      
      if (strcmp ((char*)name, "program") != 0)
       {
	idmef_parser_warning (state, "should find </program> here.  Found </%s>",
		      name);
       break;
      }

      overflowalert->program->program = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (overflowalert->program->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_OVERFLOWALERT_SIZE:
     {
      IDMEFoverflowalert *overflowalert =
        state->msg->alerts[state->msg->nalerts-1]->overflowalert;
        
      state->state = PARSER_ALERT_OVERFLOWALERT;
      
      if (strcmp ((char*)name, "size") != 0)
       {
	idmef_parser_warning (state, "should find </size> here.  Found </%s>",
		      name);
       break;
      }

      overflowalert->size->size = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (overflowalert->size->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_OVERFLOWALERT_BUFFER:
     {
      IDMEFoverflowalert *overflowalert =
        state->msg->alerts[state->msg->nalerts-1]->overflowalert;
        
      state->state = PARSER_ALERT_OVERFLOWALERT;
      
      if (strcmp ((char*)name, "buffer") != 0)
       {
	idmef_parser_warning (state, "should find </buffer> here.  Found </%s>",
		      name);
       break;
      }

      overflowalert->buffer->buffer = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (overflowalert->buffer->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_CORRELATIONALERT:
      if (strcmp ((char*)name, "CorrelationAlert") != 0)
	idmef_parser_warning (state, "should find </CorrelationAlert> here.  Found </%s>",
		      name);
      state->state = PARSER_ALERT;
      break;

    case PARSER_ALERT_CORRELATIONALERT_NAME:
     {
      IDMEFcorrelationalert *correlationalert =
        state->msg->alerts[state->msg->nalerts-1]->correlationalert;
        
      state->state = PARSER_ALERT_CORRELATIONALERT;
      
      if (strcmp ((char*)name, "name") != 0)
      
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
      correlationalert->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (correlationalert->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_ALERT_CORRELATIONALERT_ALERTIDENT:
     {
      IDMEFcorrelationalert *correlationalert =
        state->msg->alerts[state->msg->nalerts-1]->correlationalert;
        
      state->state = PARSER_ALERT_CORRELATIONALERT;
      
      if (strcmp ((char*)name, "alertident") != 0)
       {
	idmef_parser_warning (state, "should find </alertident> here.  Found </%s>",
		      name);
       break;
      }

      correlationalert->alertidents[correlationalert->nalertidents-1]->data =
        strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (correlationalert->alertidents[correlationalert->nalertidents-1]->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT:
      if (strcmp ((char*)name, "Heartbeat") != 0)
	idmef_parser_warning (state, "should find </Heartbeat> here.  Found </%s>",
		      name);
      state->state = PARSER_IDMEF_MESSAGE;
      break;

    case PARSER_HEARTBEAT_ANALYZER:
      if (strcmp ((char*)name, "Analyzer") != 0)
	idmef_parser_warning (state, "should find </Analyzer> here.  Found </%s>",
		      name);

      /* analyzer path tracking */
      if (state->analyzer_count > 1)
        state->state = PARSER_HEARTBEAT_ANALYZER; /* stay in Analyzer */
      else
        state->state = PARSER_HEARTBEAT;

      state->analyzer_count--;
      break;

    case PARSER_HEARTBEAT_ANALYZER_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER;
      break;

    case PARSER_HEARTBEAT_ANALYZER_NODE_NAME:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFnode *node = analyzer->node;

      state->state = PARSER_HEARTBEAT_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }

      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     } 
    case PARSER_HEARTBEAT_ANALYZER_NODE_LOCATION:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFnode *node = analyzer->node;

      state->state = PARSER_HEARTBEAT_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
        break;
       }

      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER_NODE;
      break;

    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_NETMASK:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFaddress *address = analyzer->node->addresses[analyzer->node->naddresses-1];

      state->state = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
        break;
       }

      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_ADDRESS:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFaddress *address = analyzer->node->addresses[analyzer->node->naddresses-1];

      state->state = PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
        break;
       }

      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER;
      break;

    case PARSER_HEARTBEAT_ANALYZER_PROCESS_NAME:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }

      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_PID:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
        break;
       }

      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_PATH:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
        break;
       }

      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_ARG:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
        break;
       }

/*      process->nargs++;
      process->args = i_renew (char*, process->args, process->nargs); */
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_ENV:
     {
      IDMEFanalyzer *analyzer = get_heartbeat_analyzer (state, -1);
      IDMEFprocess *process = analyzer->process;

      state->state = PARSER_HEARTBEAT_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "env") != 0)
       {
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
        break;
       }

/*      process->nenvs++;
      process->envs = i_renew (char*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
#if 0
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER:
      if (strcmp ((char*)name, "Analyzer") != 0)
	idmef_parser_warning (state, "should find </Analyzer> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER;
      break;

    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE:
      if (strcmp ((char*)name, "Node") != 0)
	idmef_parser_warning (state, "should find </Node> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER;
      break;

    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_NAME:
     {
      IDMEFnode *node =
           state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;

      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }

      node->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_LOCATION:
     {
      IDMEFnode *node =
           state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;

      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
      
      if (strcmp ((char*)name, "location") != 0)
       {
	idmef_parser_warning (state, "should find </location> here.  Found </%s>",
		      name);
        break;
       }

      node->location->location = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (node->location->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS:
      if (strcmp ((char*)name, "Address") != 0)
	idmef_parser_warning (state, "should find </Address> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE;
      break;

    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
     {
      IDMEFnode *node =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "netmask") != 0)
       {
	idmef_parser_warning (state, "should find </netmask> here.  Found </%s>",
		      name);
        break;
       }

      address->netmask->netmask = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->netmask->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
     {
      IDMEFnode *node =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->node;
      IDMEFaddress *address = node->addresses[node->naddresses-1];
      
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS;
      
      if (strcmp ((char*)name, "address") != 0)
       {
	idmef_parser_warning (state, "should find </address> here.  Found </%s>",
		      name);
        break;
       }

      address->address->address = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (address->address->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS:
      if (strcmp ((char*)name, "Process") != 0)
	idmef_parser_warning (state, "should find </Process> here.  Found </%s>",
		      name);
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER;
      break;

    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_NAME:
     {
      IDMEFprocess *process =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "name") != 0)
       {
	idmef_parser_warning (state, "should find </name> here.  Found </%s>",
		      name);
        break;
       }

      process->name->name = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->name->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PID:
     {
      IDMEFprocess *process =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "pid") != 0)
       {
	idmef_parser_warning (state, "should find </pid> here.  Found </%s>",
		      name);
       break;
      }

      process->pid->pid = strtoull (state->content, NULL, 0);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->pid->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PATH:
     {
      IDMEFprocess *process =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "path") != 0)
       {
	idmef_parser_warning (state, "should find </path> here.  Found </%s>",
		      name);
        break;
       }

      process->path->path = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->path->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ARG:
     {
      IDMEFprocess *process =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "arg") != 0)
       {
	idmef_parser_warning (state, "should find </arg> here.  Found </%s>",
		      name);
        break;
       }

/*      process->nargs++;
      process->args = i_renew (char*, process->args, process->nargs); */
      process->args[process->nargs-1]->arg = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->args[process->nargs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
      }
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ENV:
     {
      IDMEFprocess *process =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzer->analyzer->process;
          
      state->state = PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS;
      
      if (strcmp ((char*)name, "env") != 0)
       {
	idmef_parser_warning (state, "should find </env> here.  Found </%s>",
		      name);
        break;
       }

/*      process->nenvs++;
      process->envs = i_renew (char*, process->envs, process->nenvs); */
      process->envs[process->nenvs-1]->env = strdup (state->content);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (process->envs[process->nenvs-1]->idmefp->IDMEFXmlNodePtr,
          (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
#endif /* if 0*/
    case PARSER_HEARTBEAT_CREATETIME:
     {
      IDMEFtime *time = state->msg->heartbeats[state->msg->nheartbeats-1]->createtime;

      state->state = PARSER_HEARTBEAT;
      
      if (strcmp ((char*)name, "CreateTime") != 0)
       {
	idmef_parser_warning (state, "should find </CreateTime> here.  Found </%s>",
		      name);
       break;
      }
       
      time->string = strdup (state->content);
/*      time->tv = ??? */

      if (state->gen_idmefp)
       {
        if (!time->idmefp)
          idmef_parser_warning (state, "DTD violation: <CreateTime> missing "
          			       "ntpstamp attribute!");
        else
         {
          xmlNodeAddContent (time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
          xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
         }
       }
      break;
     }
    case PARSER_HEARTBEAT_ANALYZERTIME:
     {
      IDMEFtime *time =
        state->msg->heartbeats[state->msg->nheartbeats-1]->analyzertime;

      state->state = PARSER_HEARTBEAT;
      
      if (strcmp ((char*)name, "AnalyzerTime") != 0)
       {
	idmef_parser_warning (state, "should find </AnalyzerTime> here.  Found </%s>",
		      name);
       break;
      }

      time->string = strdup (state->content);
/*      time->tv = ??? */

      if (state->gen_idmefp)
       {
        if (!time->idmefp)
          idmef_parser_warning (state, "DTD violation: <AnalyzerTime> missing "
          			       "ntpstamp attribute!");
        else
         {
          xmlNodeAddContent (time->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
          xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
         }
       }
      break;
     }
    case PARSER_HEARTBEAT_ADDITIONALDATA:
     {
      IDMEFheartbeat *heartbeat =
         state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];
      
      state->state = PARSER_HEARTBEAT;
      
      if (strcmp ((char*)name, "AdditionalData") != 0)
       {
	idmef_parser_warning (state, "should find </AdditionalData> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
      xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_BOOLEAN:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];
      
      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "boolean") != 0)
       {
	idmef_parser_warning (state, "should find </boolean> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->boolean->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "byte") != 0)
       {
	idmef_parser_warning (state, "should find </byte> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->byte->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_CHARACTER:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];
      
      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "character") != 0)
       {
	idmef_parser_warning (state, "should find </character> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->character->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_DATE_TIME:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "date-time") != 0)
       {
	idmef_parser_warning (state, "should find </date-time> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->date_time->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_INTEGER:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "integer") != 0)
       {
	idmef_parser_warning (state, "should find </integer> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->integer->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_NTPSTAMP:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "ntpstamp") != 0)
       {
	idmef_parser_warning (state, "should find </ntpstamp> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->ntpstamp->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_PORTLIST:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "portlist") != 0)
       {
	idmef_parser_warning (state, "should find </portlist> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->portlist->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_REAL:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "real") != 0)
       {
	idmef_parser_warning (state, "should find </real> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->real->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_STRING:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "string") != 0)
       {
	idmef_parser_warning (state, "should find </string> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->string->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE_STRING:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "byte-string") != 0)
       {
	idmef_parser_warning (state, "should find </byte-string> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->byte_string->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_ADDITIONALDATA_XMLTEXT:
     {
      IDMEFheartbeat *heartbeat = state->msg->heartbeats[state->msg->nheartbeats-1];
      IDMEFadditionaldata *additionaldata =
        heartbeat->additionaldatas[heartbeat->nadditionaldatas-1];

      state->state = PARSER_HEARTBEAT_ADDITIONALDATA;
      
      if (strcmp ((char*)name, "xmltext") != 0)
       {
	idmef_parser_warning (state, "should find </xmltext> here.  Found </%s>",
		      name);
       break;
      }

      additionaldata->data = strdup (state->content);
      
      if (state->gen_idmefp)
       {
        xmlNodeAddContent (additionaldata->xmltext->idmefp->IDMEFXmlNodePtr,
                           (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }

    case PARSER_HEARTBEAT_HEARTBEATINTERVAL:
     {
      IDMEFheartbeatinterval *heartbeatinterval =
        state->msg->heartbeats[state->msg->nheartbeats-1]->heartbeatinterval;

      state->state = PARSER_HEARTBEAT;
      
      if (strcmp ((char*)name, "HeartbeatInterval") != 0)
       {
	idmef_parser_warning (state, "should find </HeartbeatInterval> here.  Found </%s>",
		      name);
       break;
      }

      heartbeatinterval->interval = strtoull (state->content, NULL, 10);

      if (state->gen_idmefp)
       {
        xmlNodeAddContent (heartbeatinterval->idmefp->IDMEFXmlNodePtr, (xmlChar*)state->content);
#ifdef IDMEF_DEBUG
        xmlDocFormatDump (stdout, state->msg->idmefp->IDMEFXmlDocPtr, 1);
#endif
       }
      break;
     }
    case PARSER_END:
      idmef_parser_warning (state, "should not be closing any elements in this state");
      break;
   } /* switch state */
   
  /* state->content no longer needed so we truncate it here */
  state->content[0] = 0;
   
 }

static void idmef_parser_characters (SAXParserState *state, const xmlChar *chars, int len)
 {
  switch (state->state)
   {
   
   /* TODO ASSET!!! */
   
    case PARSER_ALERT_ANALYZER_NODE_NAME:
    case PARSER_ALERT_ANALYZER_PROCESS_NAME:
/*    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_NAME:
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_NAME:
*/
    case PARSER_ALERT_SOURCE_USER_USERID_NAME:
    case PARSER_ALERT_SOURCE_SERVICE_NAME:
    case PARSER_ALERT_SOURCE_NODE_NAME:
    case PARSER_ALERT_SOURCE_PROCESS_NAME:
    case PARSER_ALERT_TARGET_NODE_NAME:
    case PARSER_ALERT_TARGET_PROCESS_NAME:
    case PARSER_ALERT_TARGET_USER_USERID_NAME:
    case PARSER_ALERT_TARGET_SERVICE_NAME:
    case PARSER_ALERT_TARGET_FILE_NAME:
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NAME:
    case PARSER_ALERT_TARGET_FILE_LINKAGE_NAME:
    case PARSER_ALERT_CLASSIFICATION_REFERENCE_NAME:
    case PARSER_ALERT_TOOLALERT_NAME:
    case PARSER_ALERT_CORRELATIONALERT_NAME:
    case PARSER_HEARTBEAT_ANALYZER_NODE_NAME:
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_NAME:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_NAME:
    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_NAME:
*/
    case PARSER_ALERT_ANALYZER_NODE_LOCATION:
//    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_LOCATION:
    case PARSER_ALERT_SOURCE_NODE_LOCATION:
    case PARSER_ALERT_TARGET_NODE_LOCATION:
    case PARSER_HEARTBEAT_ANALYZER_NODE_LOCATION:
//    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_LOCATION:
    case PARSER_ALERT_ANALYZER_NODE_ADDRESS_NETMASK:
//    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
    case PARSER_ALERT_SOURCE_NODE_ADDRESS_NETMASK:
    case PARSER_ALERT_TARGET_NODE_ADDRESS_NETMASK:
    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_NETMASK:
    case PARSER_ALERT_ANALYZER_NODE_ADDRESS_ADDRESS:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_NETMASK:
    case PARSER_ALERT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
*/
    case PARSER_ALERT_SOURCE_NODE_ADDRESS_ADDRESS:
    case PARSER_ALERT_TARGET_NODE_ADDRESS_ADDRESS:
    case PARSER_HEARTBEAT_ANALYZER_NODE_ADDRESS_ADDRESS:
    case PARSER_ALERT_ANALYZER_PROCESS_PID:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_NODE_ADDRESS_ADDRESS:
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PID:
*/
    case PARSER_ALERT_SOURCE_PROCESS_PID:
    case PARSER_ALERT_TARGET_PROCESS_PID:
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_PID:
    case PARSER_ALERT_ANALYZER_PROCESS_PATH:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PID:
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_PATH:
*/
    case PARSER_ALERT_SOURCE_PROCESS_PATH:
    case PARSER_ALERT_TARGET_PROCESS_PATH:
    case PARSER_ALERT_TARGET_FILE_PATH:
    case PARSER_ALERT_TARGET_FILE_LINKAGE_PATH:
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_PATH:
    case PARSER_ALERT_ANALYZER_PROCESS_ARG:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_PATH:
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ARG:
*/
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_ARG:
    case PARSER_ALERT_SOURCE_PROCESS_ARG:
    case PARSER_ALERT_TARGET_PROCESS_ARG:
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_ARG:
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_ARG:
    case PARSER_ALERT_ANALYZER_PROCESS_ENV:
/*    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ARG:
    case PARSER_ALERT_ANALYZER_ANALYZER_PROCESS_ENV:
*/
    case PARSER_ALERT_SOURCE_PROCESS_ENV:
    case PARSER_ALERT_TARGET_PROCESS_ENV:
    case PARSER_HEARTBEAT_ANALYZER_PROCESS_ENV:
//    case PARSER_HEARTBEAT_ANALYZER_ANALYZER_PROCESS_ENV:
    case PARSER_ALERT_SOURCE_USER_USERID_NUMBER:
    case PARSER_ALERT_TARGET_USER_USERID_NUMBER:
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_USERID_NUMBER:
    case PARSER_ALERT_TARGET_FILE_INODE_NUMBER:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_OID:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_OID:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMUNITY:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMUNITY:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_SECURITYNAME:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_SECURITYNAME:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTNAME:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTNAME:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_CONTEXTENGINEID:
    case PARSER_ALERT_SOURCE_SERVICE_SNMPSERVICE_COMMAND:
    case PARSER_ALERT_TARGET_SERVICE_SNMPSERVICE_COMMAND:
    case PARSER_ALERT_TOOLALERT_COMMAND:
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_URL:
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_URL:
    case PARSER_ALERT_CLASSIFICATION_REFERENCE_URL:
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_CGI:
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_CGI:
    case PARSER_ALERT_SOURCE_SERVICE_WEBSERVICE_HTTP_METHOD:
    case PARSER_ALERT_TARGET_SERVICE_WEBSERVICE_HTTP_METHOD:
    case PARSER_ALERT_SOURCE_SERVICE_PORT:
    case PARSER_ALERT_TARGET_SERVICE_PORT:
    case PARSER_ALERT_SOURCE_SERVICE_PORTLIST:
    case PARSER_ALERT_TARGET_SERVICE_PORTLIST:
    case PARSER_ALERT_SOURCE_SERVICE_PROTOCOL:
    case PARSER_ALERT_TARGET_SERVICE_PROTOCOL:
    case PARSER_ALERT_TARGET_FILE_CREATE_TIME:
    case PARSER_ALERT_TARGET_FILE_MODIFY_TIME:
    case PARSER_ALERT_TARGET_FILE_ACCESS_TIME:
    case PARSER_ALERT_TARGET_FILE_DATA_SIZE:
    case PARSER_ALERT_TARGET_FILE_DISK_SIZE:
    case PARSER_ALERT_TARGET_FILE_FILEACCESS_PERMISSION:
    case PARSER_ALERT_TARGET_FILE_INODE_CHANGE_TIME:
    case PARSER_ALERT_TARGET_FILE_INODE_MAJOR_DEVICE:
    case PARSER_ALERT_TARGET_FILE_INODE_MINOR_DEVICE:
    case PARSER_ALERT_TARGET_FILE_INODE_C_MAJOR_DEVICE:
    case PARSER_ALERT_TARGET_FILE_INODE_C_MINOR_DEVICE:
    case PARSER_ALERT_TARGET_FILE_CHECKSUM_VALUE:
    case PARSER_ALERT_TARGET_FILE_CHECKSUM_KEY:
    case PARSER_ALERT_TOOLALERT_ALERTIDENT:
    case PARSER_ALERT_CORRELATIONALERT_ALERTIDENT:
    case PARSER_ALERT_OVERFLOWALERT_PROGRAM:
    case PARSER_ALERT_OVERFLOWALERT_SIZE:
    case PARSER_ALERT_OVERFLOWALERT_BUFFER:
    case PARSER_ALERT_CREATETIME:
    case PARSER_HEARTBEAT_CREATETIME:
    case PARSER_ALERT_ANALYZERTIME:
    case PARSER_HEARTBEAT_ANALYZERTIME:
    case PARSER_ALERT_DETECTTIME:
    case PARSER_ALERT_ASSESSMENT_ACTION:
    case PARSER_ALERT_ASSESSMENT_CONFIDENCE:
    case PARSER_ALERT_ADDITIONALDATA_BOOLEAN:
    case PARSER_ALERT_ADDITIONALDATA_BYTE:
    case PARSER_ALERT_ADDITIONALDATA_CHARACTER:
    case PARSER_ALERT_ADDITIONALDATA_DATE_TIME:
    case PARSER_ALERT_ADDITIONALDATA_NTPSTAMP:
    case PARSER_ALERT_ADDITIONALDATA_PORTLIST:
    case PARSER_ALERT_ADDITIONALDATA_REAL:
    case PARSER_ALERT_ADDITIONALDATA_STRING:
    case PARSER_ALERT_ADDITIONALDATA_BYTE_STRING:
    case PARSER_ALERT_ADDITIONALDATA_XMLTEXT:
    case PARSER_HEARTBEAT_ADDITIONALDATA_BOOLEAN:
    case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE:
    case PARSER_HEARTBEAT_ADDITIONALDATA_CHARACTER:
    case PARSER_HEARTBEAT_ADDITIONALDATA_DATE_TIME:
    case PARSER_HEARTBEAT_ADDITIONALDATA_NTPSTAMP:
    case PARSER_HEARTBEAT_ADDITIONALDATA_PORTLIST:
    case PARSER_HEARTBEAT_ADDITIONALDATA_REAL:
    case PARSER_HEARTBEAT_ADDITIONALDATA_STRING:
    case PARSER_HEARTBEAT_ADDITIONALDATA_BYTE_STRING:
    case PARSER_HEARTBEAT_ADDITIONALDATA_XMLTEXT:
    case PARSER_HEARTBEAT_HEARTBEATINTERVAL:
     {
      int l = len + strlen (state->content) + 1;
      char *str = strdup (state->content);
      
      state->content = i_renew (char, state->content, l);
      
      if (!state->content) {
        idmef_parser_fatal_error (state, ENOMEM, 
                                  "Cannot allocate memory!");
	return;
      }
      snprintf (state->content, l, "%s%s", str, chars);
      free (str);
      break;
     }
    default:
      /* don't care about content in any other states */
      break;
   }
 }

static void idmef_parser_warning (SAXParserState *state, const char *format, ...)
 {
  va_list args;
  char str[1025];
  
  va_start (args, format);
  vsnprintf (str, 1024, format, args);
  fprintf (stdout, "WARNING: %s\n", str);
  va_end (args);
 }

static void idmef_parser_error (SAXParserState *state, const char *format, ...)
 {
  va_list args;
  char str[1025];
  
  va_start (args, format);
  vsnprintf (str, 1024, format, args);
  fprintf (stdout, "ERROR: %s\n", str);
  va_end (args);
 }

static int idmef_parser_fatal_error (SAXParserState *state, int errn, const char *format, ...)
 {
  va_list args;
  char str[1025];
  
  va_start (args, format);
  vsnprintf (str, 1024, format, args);
  fprintf (stdout, "FATAl ERROR: %s\n", str);
  va_end (args);
  
  errno = errn;
  return errn;
 }



/**
 * idmef_parse_file
 * @file: the filename of the XML file.
 * @domain: the translation domain for the XML file.
 * @gen_idmefp: 0 don't generate IDMEFPtr nodes, 1 generate
 *
 * This function parses an IDMEF XML file to an IDMEF
 * object
 *
 * Returns: the IDMEF structure for the XML file.
 */
IDMEFmessage* idmef_parse_file (const char *file, const char *domain,
			        const unsigned int gen_idmefp)
 {
  SAXParserState state = { 0 };
  struct stat s;

  if (stat (file, &s) == -1)
   {
    idmef_parser_warning (&state, "could not find IDMEF file '%s'", file);
    return NULL;
   }

  /* TODO load DTD and check file against it 
  
   dtd = xmlSAXParseDTD()
   
   */

//  state.idmefp = NULL;
  state.msg = NULL;
  state.gen_idmefp = gen_idmefp;

  if (domain)
    state.domain = domain;
  else
    state.domain = textdomain (NULL);

  if (xmlSAXUserParseFile (&IDMEFHandler, &state, file) < 0)
   {
    idmef_parser_warning (&state, "document not well formed");

//    if (state.idmefp)
//      libidmef_idmefp_destroy (state.idmefp);
    return NULL;
   }

  if (state.state != PARSER_END)
   {
    idmef_parser_warning (&state, "did not finish in PARSER_END state");

//    if (state.idmefp)
//      libidmef_idmefp_destroy (state.idmefp);
      
    return NULL;
   }
     
  return state.msg;
 }

/**
 * idmef_parser_parse_memory
 * @buffer: a buffer in memory containing XML data.
 * @len: the length of @buffer.
 * @domain: the translation domain for the XML file.
 * @gen_idmefp: 0 don't generate IDMEFPtr nodes, 1 generate
 *
 * This function is similar to idmef_parser_parse_file, except that it
 * parses XML data from a buffer in memory.
 *
 * Returns: the IDMEF structure for the XML buffer.
 */
IDMEFmessage* idmef_parse_memory (const char *buffer, int len, const char *domain,
				  const unsigned int gen_idmefp)
 {
  SAXParserState state = { 0 };

  /* TODO load DTD and check buffer against it */

//  state.idmefp = NULL;
  state.msg = NULL;
  state.gen_idmefp = gen_idmefp;

  if (domain)
    state.domain = domain;
  else
    state.domain = textdomain (NULL);

  if (xmlSAXUserParseMemory (&IDMEFHandler, &state, buffer, len) < 0)
   {
    idmef_parser_warning (&state, "document not well formed!");

//    if (state.idmefp)
//      libidmef_idmefp_destroy (state.idmefp);
      
    return NULL;
   }
   
  if (state.state != PARSER_END)
   {
    idmef_parser_warning (&state, "did not finish in PARSER_END state!");

//    if (state.idmefp)
//      libidmef_idmefp_destroy (state.idmefp);
      
    return NULL;
   }

  return state.msg;
 }

/**
 * Convert IDMEFmessage, e.g. generated by the sax parser, to an IDMEFPtr 
**
IDMEFPtr msg_to_ptr (IDMEFmessage *msg)
 {
  IDMEFPtr idmefp = NULL;
  
  if (!msg)
   {
#ifdef IDMEF_DEBUG
    fprintf(stdout, "msg_to_ptr: error: missing required argument\n");
#endif
    return (NULL);
   }
   
  idmefp =
    newIDMEF_Message (
      msg->version ? newIDMEF_Message_version (msg->version) : ,
      msg->attlist->xmlns ? newIDMEF_Message_xmlns
      			      (msg->attlist->xmlns) :,
      msg->attlist->xmlns_idmef ? newIDMEF_Message_xmlns_idmef
      				    (msg->attlist->xmlns_idmef) :,
      msg->attlist->xml_lang ? newIDMEF_Message_xml_lang
      				 (msg->attlist->xml_lang) :,
      msg->attlist->xml_space ? newIDMEF_Message_xml_space
                                 (msg->attlist->xml_space) :,
      NULL); 
 }
 
 
/**
 * Convert IDMEFPtr, e.g. generated by the sax parser, to an IDMEFmessage 
**
IDMEFmessage* ptr_to_msg (IDMEFPtr idmefp)
 {
  IDMEFmessage *msg = NULL;
  xmlNodePtr cur;
  
  if (!idmefp)
   {
#ifdef IDMEF_DEBUG
    fprintf(stdout, "ptr_to_message: error: missing required argument\n");
#endif
    return (NULL);
   }
   
  libidmef_idmefp_lock_op (idmefp, IDMEF_Lock_Op_READ_LOCK);
  cur = idmefp->IDMEFXmlNodePtr;

  /* cycle through the xml document *
  while (cur)
   {
    xmlNodePtr child = cur->children;
    
    
   } /* while cur *
  
  
  if (cur->children != NULL)
   {
    xmlNodePtr child = cur->children;

    while (child != NULL)
     {
      
      child = child->next;
     } /* while *
   } /* if cur->children *
  
  libidmef_idmefp_lock_op (idmefp, IDMEF_Lock_Op_READ_LOCK);
 }
 */
