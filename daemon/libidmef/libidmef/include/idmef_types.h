/*
Author: Adam C. Migus <adam@migus.org>

Copyright (c) 2001 Networks Associates Technology, Inc.
Copyright (c) 2004 Adam C. Migus.
All rights reserved.

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_types.h,v 1.8 2009/02/15 14:15:04 alcapone Exp $
*/

#ifndef _IDMEFXML_TYPES_H
#define _IDMEFXML_TYPES_H 1

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/threads.h>

/*
 * Various types to allow libidmef to encapsulate IDMEF messages and
 * manipluate them in an `intelligent' fashion.
 */

/*
 * IDMEF/XML document/node type.
 */
typedef enum _IDMEF_Type {
	IDMEF_Type_NODE		= 0,
	IDMEF_Type_DOC		= 1,
	IDMEF_Type_CONTENT	= 2,
	IDMEF_Type_ATTRIBUTE	= 3
} IDMEF_Type;

/*
 * An IDMEF structure encapsulates an IDMEF/XML document/node.
 */
typedef struct _IDMEF {
	IDMEF_Type	IDMEF_type;
	xmlNodePtr	IDMEFXmlNodePtr;
	xmlDocPtr	IDMEFXmlDocPtr;
	xmlMutexPtr	IDMEFXmlMutexPtr;
        struct _IDMEF   *next,
                        *prev;
} IDMEF;
typedef IDMEF *IDMEFPtr;

/*
 * Locking operations for IDMEF documents/nodes.
 */
typedef enum _IDMEF_Lock_Op {
	IDMEF_Lock_Op_READ_LOCK		= 0,
	IDMEF_Lock_Op_WRITE_LOCK	= 1,
	IDMEF_Lock_Op_READ_UNLOCK	= 2,
	IDMEF_Lock_Op_WRITE_UNLOCK	= 3
} IDMEF_Lock_Op;


/*
 *  Enumeration definitions.
 */

typedef enum _IDMEFspoofed {
                        SPOOFED_UNKNOWN  = 0,
			SPOOFED_YES      = 1,
			SPOOFED_NO       = 2
} IDMEFspoofed;

typedef enum _IDMEFdecoy {
                        DECOY_UNKNOWN    = 0,
		        DECOY_YES        = 1,
		        DECOY_NO         = 2
} IDMEFdecoy;

typedef enum _IDMEFaddress_category {
                        ADDRESS_CATEGORY_UNKNOWN = 0,
			ATM              = 1,
			E_MAIL           = 2,
			LOTUS_NOTES      = 3,
			MAC              = 4,
			SNA              = 5,
			VM               = 6,
			IPV4_ADDR        = 7,
			IPV4_ADDR_HEX    = 8,
			IPV4_NET         = 9,
                        IPV4_NET_MASK    = 10,
			IPV6_ADDR        = 11,
			IPV6_ADDR_HEX    = 12,
			IPV6_NET         = 13,
			IPV6_NET_MASK    = 14
} IDMEFaddress_category;

typedef enum _IDMEForigin {
                        UNKNOWN_ORIGIN   = 0,
			VENDOR_SPECIFIC  = 1,
			USER_SPECIFIC    = 2,
			BUGTRAQID        = 3,
			CVE              = 4,
                        OSVDB		 = 5
} IDMEForigin;

typedef enum _IDMEFnode_category {
                        NODE_CATEGORY_UNKNOWN = 0,
			ADS              = 1,
			AFS              = 2,
			CODA             = 3,
			DFS              = 4,
			DNS              = 5,
			HOSTS            = 6,
			KERBEROS         = 7,
			NDS              = 8,
			NIS              = 9,
			NISPLUS          = 10,
			NT               = 11,
			WFW              = 12
} IDMEFnode_category;

typedef enum _IDMEFaction_category {
			BLOCK_INSTALLED       = 0,
			NOTIFICATION_SENT     = 1,
			TAKEN_OFFLINE         = 2,
			ACTION_CATEGORY_OTHER = 3
} IDMEFaction_category;

typedef enum _IDMEFuser_category {
                        USER_CATEGORY_UNKNOWN = 0,
			APPLICATION           = 1,
			OS_DEVICE             = 2
} IDMEFuser_category;

typedef enum _IDMEFadditionaldata_type {
			BOOLEAN          = 0,
			BYTE             = 1,
			CHARACTER        = 2,
			DATETIME         = 3,
			INTEGER          = 4,
                        NTPSTAMP         = 5,
                        PORTLIST         = 6,
			REAL             = 7,
			STRING           = 8,
			BYTE_STRING      = 9,
			XMLTEXT          = 10
} IDMEFadditionaldata_type;

typedef enum _IDMEFconfidence_rating {
			CONFIDENCE_RATING_LOW     = 0,
			CONFIDENCE_RATING_MEDIUM  = 1,
			CONFIDENCE_RATING_HIGH    = 2,
                        CONFIDENCE_RATING_NUMERIC = 3
} IDMEFconfidence_rating;

typedef enum _IDMEFuserid_type {
                        CURRENT_USER     = 0,
			ORIGINAL_USER    = 1,
			TARGET_USER      = 2,
			USER_PRIVS       = 3,
			CURRENT_GROUP    = 4,
			GROUP_PRIVS      = 5,
			OTHER_PRIVS      = 6
} IDMEFuserid_type;

typedef enum _IDMEFimpact_severity {
			IMPACT_SEVERITY_INFO       = 0,
			IMPACT_SEVERITY_LOW        = 1,
			IMPACT_SEVERITY_MEDIUM     = 2,
			IMPACT_SEVERITY_HIGH       = 3
} IDMEFimpact_severity;

typedef enum _IDMEFimpact_completion {
			FAILED           = 0,
			SUCCEEDED        = 1
} IDMEFimpact_completion;

typedef enum _IDMEFimpact_type {
			ADMIN             = 0,
			DOS               = 1,
			IMPACT_TYPE_FILE  = 2,
			RECON             = 3,
                        USER              = 4,
                        IMPACT_TYPE_OTHER = 5
} IDMEFimpact_type;


typedef enum _IDMEFfile_category {
			CURRENT          = 0,
			ORIGINAL         = 1
} IDMEFfile_category;

typedef enum _IDMEFfstype {
			FSTYPE_UFS     = 0,
			FSTYPE_EFS     = 1,
                        FSTYPE_NFS     = 2,
                        FSTYPE_AFS     = 3,
                        FSTYPE_NTFS    = 4,
                        FSTYPE_FAT16   = 5,
                        FSTYPE_FAT32   = 6,
                        FSTYPE_PCFS    = 7,
                        FSTYPE_JOLIET  = 8,
                        FSTYPE_ISO9660 = 9
} IDMEFfstype;

typedef enum _IDMEFlinkage_category {
			HARD_LINK        = 0,
			MOUNT_POINT      = 1,
			REPARSE_POINT    = 2,
			SHORTCUT         = 3,
			STREAM           = 4,
			SYMBOLIC_LINK    = 5
} IDMEFlinkage_category;

typedef enum _IDMEFchecksum_algorithm {
			MD4		 = 0,
                        MD5		 = 1,
                        SHA1		 = 2,
                        SHA2_256	 = 3,
                        SHA2_384	 = 4,
                        SHA2_512	 = 5,
                        CRC_32		 = 6,
                        HAVAL		 = 7,
                        TIGER		 = 8,
                        GOST		 = 9
} IDMEFchecksum_algorithm;

/*
 *  Struct definitions
 */

typedef struct _IDMEFattlist_global {
	char *xmlns;
        char *xmlns_idmef;
        char *xml_lang;
        char *xml_space;
} IDMEFattlist_global;

typedef struct _IDMEFlocation {
	char *location;
        IDMEFPtr idmefp;
} IDMEFlocation;

typedef struct _IDMEFname {
	char *name;
        IDMEFPtr idmefp;
} IDMEFname;

typedef struct _IDMEFnetmask {
	char *netmask;
        IDMEFPtr idmefp;
} IDMEFnetmask;

typedef struct _IDMEFaddressaddress {
	char *address;
        IDMEFPtr idmefp;
} IDMEFaddressaddress;

typedef struct _IDMEFpid {
	int pid;
        IDMEFPtr idmefp;
} IDMEFpid;

typedef struct _IDMEFpath {
	char *path;
        IDMEFPtr idmefp;
} IDMEFpath;

typedef struct _IDMEFarg {
	char *arg;
        IDMEFPtr idmefp;
} IDMEFarg;

typedef struct _IDMEFenv {
	char *env;
        IDMEFPtr idmefp;
} IDMEFenv;

typedef struct _IDMEFnumber {
	int number;
        IDMEFPtr idmefp;
} IDMEFnumber;

typedef struct _IDMEFpermission {
	char *permission;
        IDMEFPtr idmefp;
} IDMEFpermission;

typedef struct _IDMEFcreate_time {
	char *create_time;
        IDMEFPtr idmefp;
} IDMEFcreate_time;

/*typedef struct _IDMEFfstype {
	char *fstype;
        IDMEFPtr idmefp;
} IDMEFfstype;
*/
typedef struct _IDMEFmodify_time {
	char *modify_time;
        IDMEFPtr idmefp;
} IDMEFmodify_time;

typedef struct _IDMEFaccess_time {
	char *access_time;
        IDMEFPtr idmefp;
} IDMEFaccess_time;

typedef struct _IDMEFdata_size {
	int data_size;
        IDMEFPtr idmefp;
} IDMEFdata_size;

typedef struct _IDMEFdisk_size {
	int disk_size;
        IDMEFPtr idmefp;
} IDMEFdisk_size;

typedef struct _IDMEFchange_time {
	char *change_time;
        IDMEFPtr idmefp;
} IDMEFchange_time;

typedef struct _IDMEFmajor_device {
	int major_device;
        IDMEFPtr idmefp;
} IDMEFmajor_device;

typedef struct _IDMEFminor_device {
	int minor_device;
        IDMEFPtr idmefp;
} IDMEFminor_device;

typedef struct _IDMEFc_major_device {
	int c_major_device;
        IDMEFPtr idmefp;
} IDMEFc_major_device;

typedef struct _IDMEFc_minor_device {
	int c_minor_device;
        IDMEFPtr idmefp;
} IDMEFc_minor_device;

typedef struct _IDMEFvalue {
	char *value;
        IDMEFPtr idmefp;
} IDMEFvalue;

typedef struct _IDMEFkey {
	char *key;
        IDMEFPtr idmefp;
} IDMEFkey;

typedef struct _IDMEFurl {
	char *url;
        IDMEFPtr idmefp;
} IDMEFurl;

typedef struct _IDMEFcommand {
	char *command;
        IDMEFPtr idmefp;
} IDMEFcommand;

typedef struct _IDMEFprogram {
	char *program;
        IDMEFPtr idmefp;
} IDMEFprogram;

typedef struct _IDMEFsize {
	int size;
        IDMEFPtr idmefp;
} IDMEFsize;

typedef struct _IDMEFbuffer {
	char *buffer;
        IDMEFPtr idmefp;
} IDMEFbuffer;

typedef struct _IDMEFport {
	int port;
        IDMEFPtr idmefp;
} IDMEFport;

typedef struct _IDMEFportlist {
	char *portlist;
        IDMEFPtr idmefp;
} IDMEFportlist;

typedef struct _IDMEFprotocol {
	char *protocol;
        IDMEFPtr idmefp;
} IDMEFprotocol;

typedef struct _IDMEFoid {
	char *oid;
        IDMEFPtr idmefp;
} IDMEFoid;

typedef struct _IDMEFcommunity {
	char *community;
        IDMEFPtr idmefp;
} IDMEFcommunity;

typedef struct _IDMEFsecurityname {
	char *securityName;
        IDMEFPtr idmefp;
} IDMEFsecurityName;

typedef struct _IDMEFcontextname {
	char *contextName;
        IDMEFPtr idmefp;
} IDMEFcontextName;

typedef struct _IDMEFcontextengineid {
	char *contextEngineID;
        IDMEFPtr idmefp;
} IDMEFcontextEngineID;

typedef struct _IDMEFcgi {
	char *cgi;
        IDMEFPtr idmefp;
} IDMEFcgi;

typedef struct _IDMEFhttp_method {
	char *http_method;
        IDMEFPtr idmefp;
} IDMEFhttp_method;

typedef struct _IDMEFboolean {
	char *boolean;
        IDMEFPtr idmefp;
} IDMEFboolean;

typedef struct _IDMEFbyte {
	char *byte;
        IDMEFPtr idmefp;
} IDMEFbyte;

typedef struct _IDMEFcharacter {
	char *character;
        IDMEFPtr idmefp;
} IDMEFcharacter;

typedef struct _IDMEFdate_time {
	char *date_time;
        IDMEFPtr idmefp;
} IDMEFdate_time;

typedef struct _IDMEFinteger {
	char *integer;
        IDMEFPtr idmefp;
} IDMEFinteger;

typedef struct _IDMEFntpstamp {
	char *ntpstamp;
        IDMEFPtr idmefp;
} IDMEFntpstamp;

typedef struct _IDMEFreal {
	char *real;
        IDMEFPtr idmefp;
} IDMEFreal;

typedef struct _IDMEFstring {
	char *string;
        IDMEFPtr idmefp;
} IDMEFstring;

typedef struct _IDMEFbyte_string {
	char *byte_string;
        IDMEFPtr idmefp;
} IDMEFbyte_string;

typedef struct _IDMEFxmltext {
	char *xmltext;
        IDMEFPtr idmefp;
} IDMEFxmltext;

/*
typedef struct _IDMEF {
	char *;
        IDMEFPtr idmefp;
} IDMEF;

*/

typedef struct _IDMEFaddress {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFaddress_category category;
	char *vlan_name;	
	int vlan_num;
	IDMEFaddressaddress *address;
	IDMEFnetmask *netmask;
        IDMEFPtr idmefp;
} IDMEFaddress;

typedef struct _IDMEFtime {
	IDMEFattlist_global *attlist;
	char *ntpstamp;
	char *string;
	struct timeval tv;
        IDMEFPtr idmefp;
} IDMEFtime;

typedef struct _IDMEFuserid {
	IDMEFattlist_global *attlist;
	char *ident;
        char *tty;
	IDMEFuserid_type type;
	IDMEFname *name;
	IDMEFnumber *number;
        IDMEFPtr idmefp;
} IDMEFuserid;
	
typedef struct _IDMEFuser {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFuser_category category;
	int nuserids;
	IDMEFuserid **userids;
        IDMEFPtr idmefp;
} IDMEFuser;

typedef struct _IDMEFsnmpservice {
	IDMEFattlist_global *attlist;
	IDMEFoid *oid;
	IDMEFcommunity *community;
        IDMEFsecurityName *securityName;
        IDMEFcontextName *contextName;
        IDMEFcontextEngineID *contextEngineID;
	IDMEFcommand *command;
        IDMEFPtr idmefp;
} IDMEFsnmpservice;

typedef struct _IDMEFwebservice {
	IDMEFattlist_global *attlist;
	IDMEFurl *url;
	IDMEFcgi *cgi;
	IDMEFhttp_method *http_method;
	int nargs;
	IDMEFarg **args;
        IDMEFPtr idmefp;
} IDMEFwebservice;

typedef struct _IDMEFservice {
	IDMEFattlist_global *attlist;
	char *ident;
        int ip_version;
        int iana_protocol_number;
        char *iana_protocol_name;
	IDMEFname *name;
	IDMEFport *port;
	IDMEFportlist *portlist;
	IDMEFprotocol *protocol;
	IDMEFwebservice *webservice;
	IDMEFsnmpservice *snmpservice;
        IDMEFPtr idmefp;
} IDMEFservice;

typedef struct _IDMEFprocess {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFname *name;
	IDMEFpid *pid;
	IDMEFpath *path;
	int nargs;
	IDMEFarg **args;
	int nenvs;
	IDMEFenv **envs;
        IDMEFPtr idmefp;
} IDMEFprocess;

typedef struct _IDMEFnode {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFnode_category category;
	IDMEFlocation *location;
	IDMEFname *name;
	int naddresses;
	IDMEFaddress **addresses;
        IDMEFPtr idmefp;
} IDMEFnode;

typedef struct _IDMEFfileaccess {
	IDMEFattlist_global *attlist;
	IDMEFuserid *userid;
	int npermissions;
	IDMEFpermission **permissions;
        IDMEFPtr idmefp;
} IDMEFfileaccess;

typedef struct _IDMEFlinkage {
	IDMEFattlist_global *attlist;
	IDMEFlinkage_category category;
	IDMEFname *name;
	IDMEFpath *path;
	struct _IDMEFfile *file;
        IDMEFPtr idmefp;
} IDMEFlinkage;

typedef struct _IDMEFinode {
	IDMEFattlist_global *attlist;
	IDMEFchange_time *change_time;
	IDMEFnumber *number;
	IDMEFmajor_device *major_device;
	IDMEFminor_device *minor_device;
	IDMEFc_major_device *c_major_device;
	IDMEFc_minor_device *c_minor_device;
        IDMEFPtr idmefp;
} IDMEFinode;

typedef struct _IDMEFfile {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFfile_category category;
	IDMEFfstype fstype;
	IDMEFname *name;
	IDMEFpath *path;
	IDMEFcreate_time *create_time;
	IDMEFmodify_time *modify_time;
	IDMEFaccess_time *access_time;
	IDMEFdata_size *data_size;
	IDMEFdisk_size *disk_size;
	int nfileaccesses;
	IDMEFfileaccess **fileaccesses;
	int nlinkages;
	IDMEFlinkage **linkages;
	IDMEFinode *inode;
        int nchecksums;
        struct _IDMEFchecksum **checksums;
        char *file_type; /* draft 13 */
        IDMEFPtr idmefp;
} IDMEFfile;

typedef struct _IDMEFsource {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFspoofed spoofed;
	char *interface;
	IDMEFnode *node;
	IDMEFuser *user;
	IDMEFprocess *process;
	IDMEFservice *service;
        IDMEFPtr idmefp;
} IDMEFsource;

typedef struct _IDMEFtarget {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFdecoy decoy;
	char *interface;
	IDMEFnode *node;
	IDMEFuser *user;
	IDMEFprocess *process;
	IDMEFservice *service;
        int nfiles;
	IDMEFfile **files;
        IDMEFPtr idmefp;
} IDMEFtarget;
	
typedef struct _IDMEFreference {
	IDMEForigin origin;
        char *meaning;
	IDMEFname *name;
	IDMEFurl *url;
        IDMEFPtr idmefp;
} IDMEFreference;

typedef struct _IDMEFclassification {
	char *ident;
	char *text;
        int nreferences;
        IDMEFreference **references;
        IDMEFPtr idmefp;
} IDMEFclassification;

typedef struct _IDMEFimpact {
	IDMEFattlist_global *attlist;
	IDMEFimpact_severity severity;
	IDMEFimpact_completion completion;
	IDMEFimpact_type type;
	char *data;
        IDMEFPtr idmefp;
} IDMEFimpact;

typedef struct _IDMEFaction {
	IDMEFattlist_global *attlist;
	IDMEFaction_category category;
	char *data;
        IDMEFPtr idmefp;
} IDMEFaction;

typedef struct _IDMEFconfidence {
	IDMEFattlist_global *attlist;
	IDMEFconfidence_rating rating;
	float data;
        IDMEFPtr idmefp;
} IDMEFconfidence;
	
typedef struct _IDMEFassessment {
	IDMEFattlist_global *attlist;
	IDMEFimpact *impact;
	int nactions;
	IDMEFaction **actions;
	IDMEFconfidence *confidence;
        IDMEFPtr idmefp;
} IDMEFassessment;

typedef struct _IDMEFadditionaldata {
	IDMEFattlist_global *attlist;
	IDMEFadditionaldata_type type;
        IDMEFboolean *boolean;
        IDMEFbyte *byte;
        IDMEFcharacter *character;
        IDMEFdate_time *date_time;
        IDMEFinteger *integer;
        IDMEFntpstamp *ntpstamp;
        IDMEFportlist *portlist;
        IDMEFreal *real;
        IDMEFstring *string;
        IDMEFbyte_string *byte_string;
        IDMEFxmltext *xmltext;
	char *meaning;
	char *data;
        IDMEFPtr idmefp;
} IDMEFadditionaldata;

typedef struct _IDMEFanalyzer {
	IDMEFattlist_global *attlist;
        int nanalyzers; /* helper counter for analyzer path tracking
        		   not defined in the draft but required for
                           implementation; can be 0 or 1 */
	char *analyzerid;
        char *name;
	char *manufacturer;
	char *model;
	char *version;
	char *analyzer_class;
	char *ostype;
	char *osversion;
	IDMEFnode *node;
	IDMEFprocess *process;
        struct _IDMEFanalyzer *analyzer;
        IDMEFPtr idmefp;
} IDMEFanalyzer;

typedef struct _IDMEFalertident {
	IDMEFattlist_global *attlist;
	char *data;
	char *analyzerid;
        IDMEFPtr idmefp;
} IDMEFalertident;

typedef struct _IDMEFtoolalert {
	IDMEFattlist_global *attlist;
	IDMEFname *name;
	IDMEFcommand *command;
	int nalertidents;
	IDMEFalertident **alertidents;
        IDMEFPtr idmefp;
} IDMEFtoolalert;

typedef struct _IDMEFoverflowalert {
	IDMEFattlist_global *attlist;
	IDMEFprogram *program;
	IDMEFsize *size;
	IDMEFbuffer *buffer;
        IDMEFPtr idmefp;
} IDMEFoverflowalert;

typedef struct _IDMEFcorrelationalert {
	IDMEFattlist_global *attlist;
	IDMEFname *name;
	int nalertidents;
	IDMEFalertident **alertidents;
        IDMEFPtr idmefp;
} IDMEFcorrelationalert;

typedef struct _IDMEFalert {
	IDMEFattlist_global *attlist;
	char *messageid;
	IDMEFanalyzer *analyzer;
	IDMEFtime *createtime;
	IDMEFclassification *classification;
	IDMEFtime *detecttime;
	IDMEFtime *analyzertime;
	int nsources;
	IDMEFsource **sources;
	int ntargets;
	IDMEFtarget **targets;
	IDMEFassessment *assessment;
	int nadditionaldatas;
	IDMEFadditionaldata **additionaldatas;
	IDMEFtoolalert *toolalert;
	IDMEFoverflowalert *overflowalert;
	IDMEFcorrelationalert *correlationalert;
        IDMEFPtr idmefp;
} IDMEFalert;

typedef struct _IMDEFheartbeatinterval {
	IDMEFattlist_global *attlist;
	int interval;
        IDMEFPtr idmefp;
} IDMEFheartbeatinterval;

typedef struct _IMDEFheartbeat {
	IDMEFattlist_global *attlist;
	char *messageid;
	IDMEFanalyzer *analyzer;
	IDMEFtime *createtime;
	IDMEFtime *analyzertime;
        IDMEFheartbeatinterval *heartbeatinterval;
	int nadditionaldatas;
	IDMEFadditionaldata **additionaldatas;
        IDMEFPtr idmefp;
} IDMEFheartbeat;

#ifdef IDMEF_ASSET
typedef struct _IDMEForgsrc {
	IDMEFattlist_global *attlist;
        int nnodes;
	IDMEFnode **nodes;
        IDMEFPtr idmefp;
} IDMEForgsrc;

typedef struct _IDMEForgtgt {
	IDMEFattlist_global *attlist;
        int nnodes;
	IDMEFnode **nodes;
        IDMEFservice *service;
        IDMEFPtr idmefp;
} IDMEForgtgt;

typedef struct _IDMEFtranssrc {
	IDMEFattlist_global *attlist;
	IDMEFnode *node;
        IDMEFPtr idmefp;
} IDMEFtranssrc;

typedef struct _IDMEFtranstgt {
	IDMEFattlist_global *attlist;
	IDMEFnode *node;
        IDMEFservice *service;
        IDMEFPtr idmefp;
} IDMEFtranstgt;

typedef struct _IDMEFnat {
	IDMEFattlist_global *attlist;
        char *ident;
	IDMEForgsrc *orgsrc;
        IDMEForgtgt *orgtgt;
        IDMEFtranssrc *transsrc;
        IDMEFtranstgt *transtgt;
        IDMEFPtr idmefp;
} IDMEFnat;

typedef struct _IDMEFfwtype {
	char *fwtype;
        IDMEFPtr idmefp;
} IDMEFfwtype;

typedef struct _IDMEFfwversion {
	char *fwversion;
        IDMEFPtr idmefp;
} IDMEFfwversion;


typedef enum _IDMEFruleaction {
	RULE_ACTION_ACCEPT     = 0,
	RULE_ACTION_DROP       = 1,
	RULE_ACTION_REJECT     = 2,
	RULE_ACTION_ACCOUNTING = 3
} IDMEFruleaction;

typedef enum _IDMEFlog {
	LOG_UNKNOWN = 0,
	LOG_NONE    = 1,
	LOG_LOG     = 2,
	LOG_USER    = 3,
	LOG_OTHER   = 4
} IDMEFlog;

typedef struct _IDMEFrule {
	IDMEFattlist_global *attlist;
	char *ident;
        IDMEFlog log;
        IDMEFruleaction action;
        char *interface;
        int nnodes;
        IDMEFnode **nodes;
        int nusers;
        IDMEFuser **users;
        int nservices;
        IDMEFservice **services;
        int nnats;
        IDMEFnat **nats;
        IDMEFfwtype *fwtype;
        IDMEFfwversion *fwversion;
        IDMEFPtr idmefp;
} IDMEFrule;

typedef struct _IDMEFpatchlevel {
	char *patchlevel;
        IDMEFPtr idmefp;
} IDMEFpatchlevel;

typedef struct _IDMEFhotfix {
	char *hotfix;
        IDMEFPtr idmefp;
} IDMEFhotfix;

typedef enum _IDMEFarch {
        ARCH_UNKNOWN = 0,
	ARCH_X86     = 1,
        ARCH_X86_64  = 2,
        ARCH_AMD     = 3,
        ARCH_AMD64   = 4,
        ARCH_PPC     = 5,
        ARCH_MIPS    = 6,
        ARCH_SPARC   = 7,
        ARCH_ARM     = 8,
        ARCH_OTHER   = 9
} IDMEFarch;

typedef enum _IDMEFfamily {
        OS_FAMILY_WINDOWS    = 0,
        OS_FAMILY_LINUX      = 1,
        OS_FAMILY_MAC        = 2,
        OS_FAMILY_OPENBSD    = 3,
        OS_FAMILY_FREEBSD    = 4,
        OS_FAMILY_NETBSD     = 5,
        OS_FAMILY_SOLARIS    = 6,
        OS_FAMILY_AIX        = 7,
        OS_FAMILY_HP_UX      = 8,
        OS_FAMILY_OS2        = 9,
        OS_FAMILY_DOS        = 10,
        OS_FAMILY_UNIX_OTHER = 11,
        OS_FAMILY_OTHER      = 12
} IDMEFfamily;

typedef struct _IDMEFos {
	IDMEFattlist_global *attlist;
        IDMEFarch arch;
        char *vendor;
        IDMEFfamily family;
        char *version;
        char *name;
        IDMEFpatchlevel *patchlevel;
        int nhotfixes;
        IDMEFhotfix **hotfixes;
        IDMEFPtr idmefp;
} IDMEFos;

typedef enum _IDMEFvulnerability_exploitable {
	VULNERABILITY_EXPLOITABLE_UNKNOWN = 0,
	VULNERABILITY_EXPLOITABLE_AVAILABLE = 1,
	VULNERABILITY_EXPLOITABLE_UNAVAILABLE = 2,
	VULNERABILITY_EXPLOITABLE_RUMORED = 3 
} IDMEFvulnerability_exploitable;

typedef enum _IDMEFvulnerability_location {
	VULNERABILITY_LOCATION_UNKNOWN = 0,
	VULNERABILITY_LOCATION_LOCAL = 1,
	VULNERABILITY_LOCATION_REMOTE = 2,
	VULNERABILITY_LOCATION_PHYSICAL = 3,
	VULNERABILITY_LOCATION_DIALUP=  4 
} IDMEFvulnerability_location;

typedef enum _IDMEFvulnerability_attacktype {
	VULNERABILITY_ATTACKTYPE_UNKNOWN = 0,
	VULNERABILITY_ATTACKTYPE_AUTH_MANAGE = 1,
	VULNERABILITY_ATTACKTYPE_CRYPT = 2,
	VULNERABILITY_ATTACKTYPE_DOS = 3,
	VULNERABILITY_ATTACKTYPE_HIJACK = 4,
	VULNERABILITY_ATTACKTYPE_INFO_DISCLOSE = 5,
	VULNERABILITY_ATTACKTYPE_INFRASTRUCT = 6,
	VULNERABILITY_ATTACKTYPE_INPUT_MANIP = 7,
	VULNERABILITY_ATTACKTYPE_MISS_CONFIG=  8,
	VULNERABILITY_ATTACKTYPE_RACE = 9,
	VULNERABILITY_ATTACKTYPE_OTHER = 10
} IDMEFvulnerability_attacktype;

typedef enum _IDMEFvulnerability_attackimpact {
	VULNERABILITY_ATTACKIMPACT_UNKNOWN = 0,
	VULNERABILITY_ATTACKIMPACT_CONFIDENTIAL = 1,
	VULNERABILITY_ATTACKIMPACT_INTEGRITY = 2,
	VULNERABILITY_ATTACKIMPACT_AVAILABLE = 3 
} IDMEFvulnerability_attackimpact;

typedef enum _IDMEFvulnerability_grade {
	VULNERABILITY_GRADE_VERIFIED = 0,
	VULNERABILITY_GRADE_MYTH = 1,
	VULNERABILITY_GRADE_BEST_PRACTICE = 2,
	VULNERABILITY_GRADE_CONCERN = 3
} IDMEFvulnerability_grade;

typedef enum _IDMEFvulnerability_web {
	VULNERABILITY_WEB_UNKNOWN = 0,
	VULNERABILITY_WEB_YES = 1,
	VULNERABILITY_WEB_NO = 2
} IDMEFvulnerability_web;

typedef struct _IDMEFexploit {
	char *exploit;
        IDMEFPtr idmefp;
} IDMEFexploit;

typedef struct _IDMEFvulnerability {
	IDMEFattlist_global *attlist;
        char *ident;
	IDMEFvulnerability_exploitable exploitable;
	IDMEFvulnerability_location location;
	IDMEFvulnerability_attacktype attacktype;
	IDMEFvulnerability_attackimpact attackimpact;
	IDMEFvulnerability_grade grade;
	IDMEFvulnerability_web web;
        int nexploits;
        IDMEFexploit **exploits;
        IDMEFPtr idmefp;
} IDMEFvulnerability;

typedef enum _IDMEFresult_severity {
	RESULT_SEVERITY_INFO   = 0,
        RESULT_SEVERITY_LOW    = 1,
        RESULT_SEVERITY_MEDIUM = 2,
        RESULT_SEVERITY_HIGH   = 3
 } IDMEFresult_severity;

typedef enum _IDMEFresult_type {
	RESULT_TYPE_AUTOMATIC = 0,
        RESULT_TYPE_MANUAL    = 1
} IDMEFresult_type;

typedef struct _IDMEFbanner {
	char *banner;
        IDMEFPtr idmefp;
} IDMEFbanner;

typedef struct _IDMEFsummary {
	char *summary;
        IDMEFPtr idmefp;
} IDMEFsummary;

typedef struct _IDMEFdescription {
	char *description;
        IDMEFPtr idmefp;
} IDMEFdescription;

typedef struct _IDMEFresult {
	IDMEFattlist_global *attlist;
        char *ident;
        IDMEFresult_severity severity;
        char *version;
        char *category;
        IDMEFresult_type type;
        IDMEFconfidence *confidence;
        IDMEFservice *service;
        IDMEFvulnerability *vulnerability;
        IDMEFos *os;
        int nrules;
        IDMEFrule **rules;
        int nreferences;
        IDMEFreference **references;
	IDMEFtime *createtime;
        IDMEFbanner *banner;
        IDMEFsummary *summary;
        IDMEFdescription *description;
        IDMEFPtr idmefp;
 } IDMEFresult;

typedef struct _IDMEFtool {
	IDMEFattlist_global *attlist;
        char *name;
        char *manufacturer;
        char *model;
        char *version;
        char *tool_class;
        char *ostype;
        char *osversion;
        IDMEFurl *url;
        int nargs;
        IDMEFarg **args;
        int nresults;
        IDMEFresult **results;
        int nadditionaldatas;
        IDMEFadditionaldata **additionaldatas;
        IDMEFPtr idmefp;
} IDMEFtool;

typedef struct _IDMEFasset {
	IDMEFattlist_global *attlist;
	char *ident;
	IDMEFanalyzer *analyzer;
	IDMEFtarget *target;
        int ntools;
        IDMEFtool **tools;
        IDMEFPtr idmefp;
} IDMEFasset;
#endif

typedef struct _IDMEFmessage {
	IDMEFattlist_global *attlist;
        char *version;
	int nalerts;
	IDMEFalert **alerts;
	int nheartbeats;
	IDMEFheartbeat **heartbeats;
#ifdef IDMEF_ASSET
	int nassets;
        IDMEFasset **assets;
#endif
        IDMEFPtr idmefp;
} IDMEFmessage;

typedef struct _IDMEFchecksum {
	IDMEFattlist_global *attlist;
	IDMEFchecksum_algorithm algorithm;
        IDMEFvalue *value;
        IDMEFkey *key;
        IDMEFPtr idmefp;
} IDMEFchecksum;

#endif /* _IDMEFXML_TYPES_H */
