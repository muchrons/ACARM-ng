/*
Author: Adam C. Migus <adam@migus.org>

Copyright (c) 2004 Adam C. Migus.  All rights reserved

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: strings.c,v 1.8 2009/02/15 14:15:04 alcapone Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <idmef_types.h>
#include <idmef_global.h>
#include <idmef_strings.h>

char *
node_category_as_string(IDMEFnode_category category) {
	switch(category) {
	case NODE_CATEGORY_UNKNOWN:
		return "unknown";
	case ADS:
		return "ads";
	case AFS:
		return "afs";
	case CODA:
		return "coda";
	case DFS:
		return "dfs";
	case DNS:
		return "dns";
	case HOSTS:
		return "hosts";
	case KERBEROS:
		return "kerberos";
	case NDS:
		return "nds";
	case NIS:
		return "nis";
	case NISPLUS:
		return "nisplus";
	case NT:
		return "nt";
	case WFW:
		return "wfw";
	}

	return 0;
}

IDMEFnode_category
get_node_category(char *p) {
	if (!strcmp(p, "unknown")) {
		return NODE_CATEGORY_UNKNOWN;
	}
	else
	if (!strcmp(p, "ads")) {
		return ADS;
	}
	else
	if (!strcmp(p, "afs")) {
		return AFS;
	}
	else
	if (!strcmp(p, "coda")) {
		return CODA;
	}
	else
	if (!strcmp(p, "dfs")) {
		return DFS;
	}
	else
	if (!strcmp(p, "dns")) {
		return DNS;
	}
	else
	if (!strcmp(p, "hosts")) {
		return HOSTS;
	}
	else
	if (!strcmp(p, "kerberos")) {
		return KERBEROS;
	}
	else
	if (!strcmp(p, "nds")) {
		return NDS;
	}
	else
	if (!strcmp(p, "nis")) {
		return NIS;
	}
	else
	if (!strcmp(p, "nisplus")) {
		return NISPLUS;
	}
	else
	if (!strcmp(p, "nt")) {
		return NT;
	}
	else
	if (!strcmp(p, "wfw")) {
		return WFW;
	}

	return -1;
}

char *
address_category_as_string(IDMEFaddress_category category) {
	switch(category) {
	case ADDRESS_CATEGORY_UNKNOWN:
		return "unknown";
	case ATM:
		return "atm";
	case E_MAIL:
		return "e-mail";
	case LOTUS_NOTES:
		return "lotus-notes";
	case MAC:
		return "mac";
	case SNA:
		return "sna";
	case VM:
		return "vm";
	case IPV4_ADDR:
		return "ipv4-addr";
	case IPV4_ADDR_HEX:
		return "ipv4-addr-hex";
	case IPV4_NET:
		return "ipv4-net";
	case IPV4_NET_MASK:
		return "ipv4-net-mask";
	case IPV6_ADDR:
		return "ipv6-addr";
	case IPV6_ADDR_HEX:
		return "ipv6-addr-hex";
	case IPV6_NET:
		return "ipv6-net";
	case IPV6_NET_MASK:
		return "ipv6-net-mask";
	}

	return 0;
}

IDMEFaddress_category
get_address_category(char *p) {
	if (!strcmp(p, "unknown")) {
		return ADDRESS_CATEGORY_UNKNOWN;
	}
	else
	if (!strcmp(p, "atm")) {
		return ATM;
	}
	else
	if (!strcmp(p, "e-mail")) {
		return E_MAIL;
	}
	else
	if (!strcmp(p, "lotus-notes")) {
		return LOTUS_NOTES;
	}
	else
	if (!strcmp(p, "mac")) {
		return MAC;
	}
	else
	if (!strcmp(p, "sna")) {
		return SNA;
	}
	else
	if (!strcmp(p, "vm")) {
		return VM;
	}
	else
	if (!strcmp(p, "ipv4-addr")) {
		return IPV4_ADDR;
	}
	else
	if (!strcmp(p, "ipv4-addr-hex")) {
		return IPV4_ADDR_HEX;
	}
	else
	if (!strcmp(p, "ipv4-net")) {
		return IPV4_NET;
	}
	else
	if (!strcmp(p, "ipv4-net-mask")) {
		return IPV4_NET_MASK;
	}
	else
	if (!strcmp(p, "ipv6-addr")) {
		return IPV6_ADDR;
	}
	else
	if (!strcmp(p, "ipv6-addr-hex")) {
		return IPV6_ADDR_HEX;
	}
	else
	if (!strcmp(p, "ipv6-net")) {
		return IPV6_NET;
	}
	else
	if (!strcmp(p, "ipv6-net-mask")) {
		return IPV6_NET_MASK;
	}

	return -1;
}

IDMEFspoofed
get_spoofed(char *p) {
	if (!strcmp(p, "yes")) {
		return SPOOFED_YES;
	}
	else
	if (!strcmp(p, "no")) {
		return SPOOFED_NO;
	}
	else
	if (!strcmp(p, "unknown")) {
		return SPOOFED_UNKNOWN;
	}

	return -1;
}

char *
spoofed_as_string(IDMEFspoofed spoofed) {
	switch(spoofed) {
	case SPOOFED_YES:
		return "yes";
	case SPOOFED_NO:
		return "no";
	case SPOOFED_UNKNOWN:
		return "unknown";
	}

	return 0;
}

IDMEFdecoy
get_decoy(char *p) {
	if (!strcmp(p, "yes")) {
		return DECOY_YES;
	}
	else
	if (!strcmp(p, "no")) {
		return DECOY_NO;
	}
	else
	if (!strcmp(p, "unknown")) {
		return DECOY_UNKNOWN;
	}

	return -1;
}

char *
decoy_as_string(IDMEFdecoy decoy) {
	switch(decoy) {
	case DECOY_YES:
		return "yes";
	case DECOY_NO:
		return "no";
	case DECOY_UNKNOWN:
		return "unknown";
	}

	return 0;
}

IDMEFuser_category
get_user_category(char *p) {
	if (!strcmp(p, "unknown")) {
		return USER_CATEGORY_UNKNOWN;
	}
	else
	if (!strcmp(p, "application")) {
		return APPLICATION;
	}
	else
	if (!strcmp(p, "os-device")) {
		return OS_DEVICE;
	}

	return -1;
}

char *
user_category_as_string(IDMEFuser_category category) {
	switch(category) {
	case USER_CATEGORY_UNKNOWN:
		return "unknown";
	case APPLICATION:
		return "application";
	case OS_DEVICE:
		return "os-device";
	}

	return 0;
}

IDMEFuserid_type
get_userid_type(char *p) {
	if (!strcmp(p, "current-user")) {
		return CURRENT_USER;
	}
	else
	if (!strcmp(p, "original-user")) {
		return ORIGINAL_USER;
	}
	else
	if (!strcmp(p, "target-user")) {
		return TARGET_USER;
	}
	else
	if (!strcmp(p, "user-privs")) {
		return USER_PRIVS;
	}
	else
	if (!strcmp(p, "current-group")) {
		return CURRENT_GROUP;
	}
	else
	if (!strcmp(p, "group-privs")) {
		return GROUP_PRIVS;
	}
	else
	if (!strcmp(p, "other-privs")) {
		return OTHER_PRIVS;
	}

	return -1;
}

char *
userid_type_as_string(IDMEFuserid_type type) {
	switch(type) {
	case CURRENT_USER:
		return "current-user";
	case ORIGINAL_USER:
		return "original-user";
	case TARGET_USER:
		return "target-user";
	case USER_PRIVS:
		return "user-privs";
	case CURRENT_GROUP:
		return "current-group";
	case GROUP_PRIVS:
		return "group-privs";
	case OTHER_PRIVS:
		return "other-privs";
	}

	return 0;
}

IDMEFadditionaldata_type
get_additionaldata_type(char *p) {
	if (!strcmp(p, "boolean")) {
		return BOOLEAN;
	}
	else
	if (!strcmp(p, "byte")) {
		return BYTE;
	}
	else
	if (!strcmp(p, "character")) {
		return CHARACTER;
	}
	else
	if (!strcmp(p, "date-time")) {
		return DATETIME;
	}
	else
	if (!strcmp(p, "integer")) {
		return INTEGER;
	}
	else
	if (!strcmp(p, "ntpstamp")) {
		return NTPSTAMP;
	}
	else
	if (!strcmp(p, "portlist")) {
		return PORTLIST;
	}
	else
	if (!strcmp(p, "real")) {
		return REAL;
	}
	else
	if (!strcmp(p, "string")) {
		return STRING;
	}
	else
	if (!strcmp(p, "byte-string")) {
		return BYTE_STRING;
	}
	else
	if (!strcmp(p, "xmltext")) {
		return XMLTEXT;
	}

	return -1;
}

char *
additionaldata_type_as_string(IDMEFadditionaldata_type type) {
	switch(type) {
	case BOOLEAN:
		return "boolean";
	case BYTE:
		return "byte";
	case CHARACTER:
		return "character";
	case DATETIME:
		return "date-time";
	case INTEGER:
		return "integer";
	case NTPSTAMP:
		return "ntpstamp";
	case PORTLIST:
		return "portlist";
	case REAL:
		return "real";
	case STRING:
		return "string";
	case BYTE_STRING:
		return "byte-string";
	case XMLTEXT:
		return "xmltext";
	}

	return 0;
}

IDMEFimpact_severity
get_impact_severity(char *p) {
	if (!strcmp(p, "info")) {
		return IMPACT_SEVERITY_INFO;
	}
	else
	if (!strcmp(p, "low")) {
		return IMPACT_SEVERITY_LOW;
	}
	else
	if (!strcmp(p, "medium")) {
		return IMPACT_SEVERITY_MEDIUM;
	}
	else
	if (!strcmp(p, "high")) {
		return IMPACT_SEVERITY_HIGH;
	}

	return -1;
}

char *
impact_severity_as_string(IDMEFimpact_severity severity) {
	switch(severity) {
	case IMPACT_SEVERITY_INFO:
		return "info";
	case IMPACT_SEVERITY_LOW:
		return "low";
	case IMPACT_SEVERITY_MEDIUM:
		return "medium";
	case IMPACT_SEVERITY_HIGH:
		return "high";
	}

	return 0;
}

IDMEFimpact_completion
get_impact_completion(char *p) {
	if (!strcmp(p, "failed")) {
		return FAILED;
	}
	else
	if (!strcmp(p, "succeeded")) {
		return SUCCEEDED;
	}

	return -1;
}

char *
impact_completion_as_string(IDMEFimpact_completion completion) {
	switch(completion) {
	case FAILED:
		return "failed";
	case SUCCEEDED:
		return "succeeded";
	}

	return 0;
}

IDMEFimpact_type
get_impact_type(char *p) {
	if (!strcmp(p, "admin")) {
		return ADMIN;
	}
	else
	if (!strcmp(p, "dos")) {
		return DOS;
	}
	else
	if (!strcmp(p, "file")) {
		return IMPACT_TYPE_FILE;
	}
	else
	if (!strcmp(p, "recon")) {
		return RECON;
	}
	else
	if (!strcmp(p, "user")) {
		return USER;
	}
	else
	if (!strcmp(p, "other")) {
		return IMPACT_TYPE_OTHER;
	}

	return -1;
}

char *
impact_type_as_string(IDMEFimpact_type type) {
	switch(type) {
	case ADMIN:
		return "admin";
	case DOS:
		return "dos";
	case IMPACT_TYPE_FILE:
		return "file";
	case RECON:
		return "recon";
	case USER:
		return "user";
	case IMPACT_TYPE_OTHER:
		return "other";
	}

	return 0;
}

IDMEFaction_category
get_action_category(char *p) {
	if (!strcmp(p, "other")) {
		return ACTION_CATEGORY_OTHER;
	}
	else
	if (!strcmp(p, "block-installed")) {
		return BLOCK_INSTALLED;
	}
	else
	if (!strcmp(p, "notification-sent")) {
		return NOTIFICATION_SENT;
	}
	else
	if (!strcmp(p, "taken-offline")) {
		return TAKEN_OFFLINE;
	}

	return -1;
}

char *
action_category_as_string(IDMEFaction_category category) {
	switch(category) {
	case ACTION_CATEGORY_OTHER:
		return "other";
	case BLOCK_INSTALLED:
		return "block-installed";
	case NOTIFICATION_SENT:
		return "notification-sent";
	case TAKEN_OFFLINE:
		return "taken-offline";
	}

	return 0;
}

IDMEFconfidence_rating
get_confidence_rating(char *p) {
	if (!strcmp(p, "low")) {
		return CONFIDENCE_RATING_LOW;
	}
	else
	if (!strcmp(p, "medium")) {
		return CONFIDENCE_RATING_MEDIUM;
	}
	else
	if (!strcmp(p, "high")) {
		return CONFIDENCE_RATING_HIGH;
	}
	else
	if (!strcmp(p, "numeric")) {
		return CONFIDENCE_RATING_NUMERIC;
	}

	return -1;
}

char *
confidence_rating_as_string(IDMEFconfidence_rating rating) {
	switch(rating) {
	case CONFIDENCE_RATING_LOW:
		return "low";
	case CONFIDENCE_RATING_MEDIUM:
		return "medium";
	case CONFIDENCE_RATING_HIGH:
		return "high";
	case CONFIDENCE_RATING_NUMERIC:
		return "numeric";
	}

	return 0;
}

IDMEFfile_category
get_file_category(char *p) {
	if (!strcmp(p, "current")) {
		return CURRENT;
	}
	else
	if (!strcmp(p, "original")) {
		return ORIGINAL;
	}

	return -1;
}

char *
file_category_as_string(IDMEFfile_category category) {
	switch(category) {
	case CURRENT:
		return "current";
	case ORIGINAL:
		return "original";
	}

	return 0;
}

IDMEFfstype
get_file_fstype(char *p) {
	if (!strcmp(p, "ufs")) {
		return FSTYPE_UFS;
	}
	else
	if (!strcmp(p, "efs")) {
		return FSTYPE_NFS;
	}
	else
	if (!strcmp(p, "nfs")) {
		return FSTYPE_NFS;
	}
	else
	if (!strcmp(p, "afs")) {
		return FSTYPE_AFS;
	}
	else
	if (!strcmp(p, "ntfs")) {
		return FSTYPE_NTFS;
	}
	else
	if (!strcmp(p, "fat16")) {
		return FSTYPE_FAT16;
	}
	else
	if (!strcmp(p, "fat32")) {
		return FSTYPE_FAT32;
	}
	else
	if (!strcmp(p, "pcfs")) {
		return FSTYPE_PCFS;
	}
	else
	if (!strcmp(p, "joliet")) {
		return FSTYPE_JOLIET;
	}
	else
	if (!strcmp(p, "iso9660")) {
		return FSTYPE_ISO9660;
	}

	return -1;
}

char *
file_fstype_as_string(IDMEFfstype fstype) {
	switch(fstype) {
	case FSTYPE_UFS:
		return "ufs";
	case FSTYPE_EFS:
		return "efs";
	case FSTYPE_NFS:
		return "nfs";
	case FSTYPE_AFS:
		return "afs";
	case FSTYPE_NTFS:
		return "ntfs";
	case FSTYPE_FAT16:
		return "fat16";
	case FSTYPE_FAT32:
		return "fat32";
	case FSTYPE_PCFS:
		return "pcfs";
	case FSTYPE_JOLIET:
		return "joliet";
	case FSTYPE_ISO9660:
		return "iso9660";
	}

	return 0;
}

IDMEFlinkage_category
get_linkage_category(char *p) {
	if (!strcmp(p, "hard-link")) {
		return HARD_LINK;
	}
	else
	if (!strcmp(p, "mount-point")) {
		return MOUNT_POINT;
	}
	else
	if (!strcmp(p, "reparse-point")) {
		return REPARSE_POINT;
	}
	else
	if (!strcmp(p, "shortcut")) {
		return SHORTCUT;
	}
	else
	if (!strcmp(p, "stream")) {
		return STREAM;
	}
	else
	if (!strcmp(p, "symbolic-link")) {
		return SYMBOLIC_LINK;
	}

	return -1;
}

char *
linkage_category_as_string(IDMEFlinkage_category category) {
	switch(category) {
	case HARD_LINK:
		return "hard-link";
	case MOUNT_POINT:
		return "mount-point";
	case REPARSE_POINT:
		return "reparse-point";
	case SHORTCUT:
		return "shortcut";
	case STREAM:
		return "stream";
	case SYMBOLIC_LINK:
		return "symbolic-link";
	}

	return 0;
}

/* added for draft 12 */
IDMEForigin
get_reference_origin(char *p) {
	if (!strcmp(p, "unknown")) {
		return UNKNOWN_ORIGIN;
	}
	else
	if (!strcmp(p, "vendor-specific")) {
		return VENDOR_SPECIFIC;
	}
	else
	if (!strcmp(p, "user-specific")) {
		return USER_SPECIFIC;
	}
	else
	if (!strcmp(p, "bugtraqid")) {
		return BUGTRAQID;
	}
	else
	if (!strcmp(p, "cve")) {
		return CVE;
	}
	else
	if (!strcmp(p, "osvdb")) {
		return OSVDB;
	}

	return -1;
}

char *reference_origin_as_string(IDMEForigin origin) {
	switch(origin) {
	case UNKNOWN_ORIGIN:
		return "unknown";
	case VENDOR_SPECIFIC:
		return "vendor-specific";
	case USER_SPECIFIC:
		return "user-specific";
	case BUGTRAQID:
		return "bugtraqid";
	case CVE:
		return "cve";
	case OSVDB:
		return "osvdb";
	}

	return 0;
}

IDMEFchecksum_algorithm
get_checksum_algorithm(char *p) {
	if (!strcmp(p, "MD4")) {
		return MD4;
	}
	else
	if (!strcmp(p, "MD5")) {
		return MD5;
	}
	else
	if (!strcmp(p, "SHA1")) {
		return SHA1;
	}
	else
	if (!strcmp(p, "SHA2-256")) {
		return SHA2_256;
	}
	else
	if (!strcmp(p, "SHA2-384")) {
		return SHA2_384;
	}
	else
	if (!strcmp(p, "SHA2-512")) {
		return SHA2_512;
	}
	else
	if (!strcmp(p, "CRC-32")) {
		return CRC_32;
	}
	else
	if (!strcmp(p, "Haval")) {
		return HAVAL;
	}
	else
	if (!strcmp(p, "Tiger")) {
		return TIGER;
	}
	else
	if (!strcmp(p, "Gost")) {
		return GOST;
	}

	return -1;
}

char *
checksum_algorithm_as_string(IDMEFchecksum_algorithm algorithm) {
	switch(algorithm) {
	case MD4:
		return "MD4";
	case MD5:
		return "MD5";
	case SHA1:
		return "SHA1";
	case SHA2_256:
		return "SHA2-256";
	case SHA2_384:
		return "SHA2-384";
	case SHA2_512:
		return "SHA2-512";
	case CRC_32:
		return "CRC-32";
	case HAVAL:
		return "Haval";
	case TIGER:
		return "Tiger";
	case GOST:
		return "Gost";
	}

	return 0;
}

#ifdef IDMEF_ASSET
IDMEFruleaction get_rule_action(char *p) {
	if (!strcmp(p, "accept")) {
		return RULE_ACTION_ACCEPT;
	}
	else
	if (!strcmp(p, "drop")) {
		return RULE_ACTION_DROP;
	}
	else
	if (!strcmp(p, "reject")) {
		return RULE_ACTION_REJECT;
	}
	else
	if (!strcmp(p, "accounting")) {
		return RULE_ACTION_ACCOUNTING;
	}

	return -1;

}

char * rule_action_as_string(IDMEFruleaction action) {
	switch(action) {
	case RULE_ACTION_ACCEPT:
		return "accept";
	case RULE_ACTION_DROP:
		return "drop";
	case RULE_ACTION_REJECT:
		return "reject";
	case RULE_ACTION_ACCOUNTING:
		return "accounting";
	}

	return 0;

}


IDMEFlog get_rule_log(char *p) {
	if (!strcmp(p, "unknown")) {
		return LOG_UNKNOWN;
	}
	else
	if (!strcmp(p, "none")) {
		return LOG_NONE;
	}
	else
	if (!strcmp(p, "log")) {
		return LOG_LOG;
	}
	else
	if (!strcmp(p, "user")) {
		return LOG_USER;
	}
	else
	if (!strcmp(p, "other")) {
		return LOG_OTHER;
	}

	return -1;

}

char * rule_log_as_string(IDMEFlog log) {
	switch(log) {
	case LOG_UNKNOWN:
		return "unknown";
	case LOG_NONE:
		return "none";
	case LOG_LOG:
		return "log";
	case LOG_USER:
		return "user";
	case LOG_OTHER:
		return "other";
	}

	return 0;

}


IDMEFarch get_os_arch(char *p) {
	if (!strcmp(p, "unknown")) {
		return ARCH_UNKNOWN;
	}
	else
	if (!strcmp(p, "x86")) {
		return ARCH_X86;
	}
	else
	if (!strcmp(p, "x86-64")) {
		return ARCH_X86_64;
	}
	else
	if (!strcmp(p, "amd")) {
		return ARCH_AMD;
	}
	else
	if (!strcmp(p, "amd64")) {
		return ARCH_AMD64;
	}
	else
	if (!strcmp(p, "ppc")) {
		return ARCH_PPC;
	}
	else
	if (!strcmp(p, "mips")) {
		return ARCH_MIPS;
	}
	else
	if (!strcmp(p, "sparc")) {
		return ARCH_SPARC;
	}
	else
	if (!strcmp(p, "arm")) {
		return ARCH_ARM;
	}
	else
	if (!strcmp(p, "other")) {
		return ARCH_OTHER;
	}

	return -1;

}

char * os_arch_as_string(IDMEFarch arch) {
	switch(arch) {
	case ARCH_UNKNOWN:
		return "unknown";
	case ARCH_X86:
		return "x86";
	case ARCH_X86_64:
		return "x86-64";
	case ARCH_AMD:
		return "amd";
	case ARCH_AMD64:
		return "amd64";
	case ARCH_PPC:
		return "ppc";
	case ARCH_MIPS:
		return "mips";
	case ARCH_SPARC:
		return "sparc";
	case ARCH_ARM:
		return "arm";
	case ARCH_OTHER:
		return "other";
	}

	return 0;

}

IDMEFarch get_os_family(char *p) {
	if (!strcmp(p, "Windows")) {
		return OS_FAMILY_WINDOWS;
	}
	else
	if (!strcmp(p, "Linux")) {
		return OS_FAMILY_LINUX;
	}
	else
	if (!strcmp(p, "Mac")) {
		return OS_FAMILY_MAC;
	}
	else
	if (!strcmp(p, "OpenBSD")) {
		return OS_FAMILY_OPENBSD;
	}
	else
	if (!strcmp(p, "FreeBSD")) {
		return OS_FAMILY_FREEBSD;
	}
	else
	if (!strcmp(p, "NetBSD")) {
		return OS_FAMILY_NETBSD;
	}
	else
	if (!strcmp(p, "Solaris")) {
		return OS_FAMILY_SOLARIS;
	}
	else
	if (!strcmp(p, "AIX")) {
		return OS_FAMILY_AIX;
	}
	else
	if (!strcmp(p, "HP-UX")) {
		return OS_FAMILY_HP_UX;
	}
	else
	if (!strcmp(p, "OS2")) {
		return OS_FAMILY_OS2;
	}
	else
	if (!strcmp(p, "DOS")) {
		return OS_FAMILY_DOS;
	}
	else
	if (!strcmp(p, "Unix-other")) {
		return OS_FAMILY_UNIX_OTHER;
	}
	else
	if (!strcmp(p, "other")) {
		return OS_FAMILY_OTHER;
	}
	
	return -1;

}

char * os_family_as_string(IDMEFfamily family) {
	switch(family) {
	case OS_FAMILY_WINDOWS:
		return "Windows";
	case OS_FAMILY_LINUX:
		return "Linux";
	case OS_FAMILY_MAC:
		return "Mac";
	case OS_FAMILY_OPENBSD:
		return "OpenBSD";
	case OS_FAMILY_FREEBSD:
		return "FreeBSD";
	case OS_FAMILY_NETBSD:
		return "NetBSD";
	case OS_FAMILY_SOLARIS:
		return "Solaris";
	case OS_FAMILY_AIX:
		return "AIX";
	case OS_FAMILY_HP_UX:
		return "HP-UX";
	case OS_FAMILY_OS2:
		return "OS2";
	case OS_FAMILY_DOS:
		return "DOS";
	case OS_FAMILY_UNIX_OTHER:
		return "Unix-other";
	case OS_FAMILY_OTHER:
		return "other";
	}

	return 0;

}


IDMEFvulnerability_exploitable get_vulnerability_exploitable(char *p) {
	if (!strcmp(p, "unknown")) {
		return VULNERABILITY_EXPLOITABLE_UNKNOWN;
	}
	else
	if (!strcmp(p, "available")) {
		return VULNERABILITY_EXPLOITABLE_AVAILABLE;
	}
	else
	if (!strcmp(p, "unavailable")) {
		return VULNERABILITY_EXPLOITABLE_UNAVAILABLE;
	}
	else
	if (!strcmp(p, "rumored")) {
		return VULNERABILITY_EXPLOITABLE_RUMORED;
	}

	return -1;

}

char * vulnerability_exploitable_as_string(IDMEFvulnerability_exploitable exploitable) {
	switch(exploitable) {
	case VULNERABILITY_EXPLOITABLE_UNKNOWN:
		return "unknown";
	case VULNERABILITY_EXPLOITABLE_AVAILABLE:
		return "available";
	case VULNERABILITY_EXPLOITABLE_UNAVAILABLE:
		return "unavailable";
	case VULNERABILITY_EXPLOITABLE_RUMORED:
		return "rumored";
	}

	return 0;

}


IDMEFvulnerability_location get_vulnerability_location(char *p) {
	if (!strcmp(p, "unknown")) {
		return VULNERABILITY_LOCATION_UNKNOWN;
	}
	else
	if (!strcmp(p, "local")) {
		return VULNERABILITY_LOCATION_LOCAL;
	}
	else
	if (!strcmp(p, "remote")) {
		return VULNERABILITY_LOCATION_REMOTE;
	}
	else
	if (!strcmp(p, "physical")) {
		return VULNERABILITY_LOCATION_PHYSICAL;
	}
	else
	if (!strcmp(p, "dialup")) {
		return VULNERABILITY_LOCATION_DIALUP;
	}

	return -1;

}

char * vulnerability_location_as_string(IDMEFvulnerability_location location) {
	switch(location) {
	case VULNERABILITY_LOCATION_UNKNOWN:
		return "unknown";
	case VULNERABILITY_LOCATION_LOCAL:
		return "local";
	case VULNERABILITY_LOCATION_REMOTE:
		return "remote";
	case VULNERABILITY_LOCATION_PHYSICAL:
		return "physical";
	case VULNERABILITY_LOCATION_DIALUP:
		return "dialup";
	}

	return 0;

}


IDMEFvulnerability_attacktype get_vulnerability_attacktype(char *p) {
	if (!strcmp(p, "unknown")) {
		return VULNERABILITY_ATTACKTYPE_UNKNOWN;
	}
	else
	if (!strcmp(p, "auth_manage")) {
		return VULNERABILITY_ATTACKTYPE_AUTH_MANAGE;
	}
	else
	if (!strcmp(p, "crypt")) {
		return VULNERABILITY_ATTACKTYPE_CRYPT;
	}
	else
	if (!strcmp(p, "dos")) {
		return VULNERABILITY_ATTACKTYPE_DOS;
	}
	else
	if (!strcmp(p, "hijack")) {
		return VULNERABILITY_ATTACKTYPE_HIJACK;
	}
	else
	if (!strcmp(p, "info_disclosure")) {
		return VULNERABILITY_ATTACKTYPE_INFO_DISCLOSE;
	}
	else
	if (!strcmp(p, "infrastruct")) {
		return VULNERABILITY_ATTACKTYPE_INFRASTRUCT;
	}
	else
	if (!strcmp(p, "input_manip")) {
		return VULNERABILITY_ATTACKTYPE_INPUT_MANIP;
	}
	else
	if (!strcmp(p, "miss_config")) {
		return VULNERABILITY_ATTACKTYPE_MISS_CONFIG;
	}
	else
	if (!strcmp(p, "race")) {
		return VULNERABILITY_ATTACKTYPE_RACE;
	}
	else
	if (!strcmp(p, "other")) {
		return VULNERABILITY_ATTACKTYPE_OTHER;
	}

	return -1;

}

char * vulnerability_attacktype_as_string(IDMEFvulnerability_attacktype attacktype) {
	switch(attacktype) {
	case VULNERABILITY_ATTACKTYPE_UNKNOWN:
		return "unknown";
	case VULNERABILITY_ATTACKTYPE_AUTH_MANAGE:
		return "auth_manage";
	case VULNERABILITY_ATTACKTYPE_CRYPT:
		return "crypt";
	case VULNERABILITY_ATTACKTYPE_DOS:
		return "dos";
	case VULNERABILITY_ATTACKTYPE_HIJACK:
		return "hijack";
	case VULNERABILITY_ATTACKTYPE_INFO_DISCLOSE:
		return "disclose";
	case VULNERABILITY_ATTACKTYPE_INFRASTRUCT:
		return "infrastruct";
	case VULNERABILITY_ATTACKTYPE_INPUT_MANIP:
		return "input_manip";
	case VULNERABILITY_ATTACKTYPE_MISS_CONFIG:
		return "miss_config";
	case VULNERABILITY_ATTACKTYPE_RACE:
		return "race";
	case VULNERABILITY_ATTACKTYPE_OTHER:
		return "other";
	}

	return 0;

}


IDMEFvulnerability_attackimpact get_vulnerability_attackimpact(char *p) {
	if (!strcmp(p, "unknown")) {
		return VULNERABILITY_ATTACKIMPACT_UNKNOWN;
	}
	else
	if (!strcmp(p, "confidential")) {
		return VULNERABILITY_ATTACKIMPACT_CONFIDENTIAL;
	}
	else
	if (!strcmp(p, "integrity")) {
		return VULNERABILITY_ATTACKIMPACT_INTEGRITY;
	}
	else
	if (!strcmp(p, "available")) {
		return VULNERABILITY_ATTACKIMPACT_AVAILABLE;
	}

	return -1;

}

char * vulnerability_attackimpact_as_string(IDMEFvulnerability_attackimpact attackimpact) {
	switch(attackimpact) {
	case VULNERABILITY_ATTACKIMPACT_UNKNOWN:
		return "unknown";
	case VULNERABILITY_ATTACKIMPACT_CONFIDENTIAL:
		return "confidential";
	case VULNERABILITY_ATTACKIMPACT_INTEGRITY:
		return "integrity";
	case VULNERABILITY_ATTACKIMPACT_AVAILABLE:
		return "available";
	}

	return 0;

}


IDMEFvulnerability_grade get_vulnerability_grade(char *p) {
	if (!strcmp(p, "verified")) {
		return VULNERABILITY_GRADE_VERIFIED;
	}
	else
	if (!strcmp(p, "myth")) {
		return VULNERABILITY_GRADE_MYTH;
	}
	else
	if (!strcmp(p, "practice")) {
		return VULNERABILITY_GRADE_BEST_PRACTICE;
	}
	else
	if (!strcmp(p, "concern")) {
		return VULNERABILITY_GRADE_CONCERN;
	}

	return -1;

}

char * vulnerability_grade_as_string(IDMEFvulnerability_grade grade) {
	switch(grade) {
	case VULNERABILITY_GRADE_VERIFIED:
		return "verified";
	case VULNERABILITY_GRADE_MYTH:
		return "myth";
	case VULNERABILITY_GRADE_BEST_PRACTICE:
		return "practice";
	case VULNERABILITY_GRADE_CONCERN:
		return "concern";
	}

	return 0;

}


IDMEFvulnerability_web get_vulnerability_web(char *p) {
	if (!strcmp(p, "unknown")) {
		return VULNERABILITY_WEB_UNKNOWN;
	}
	else
	if (!strcmp(p, "yes")) {
		return VULNERABILITY_WEB_YES;
	}
	else
	if (!strcmp(p, "no")) {
		return VULNERABILITY_WEB_NO;
	}

	return -1;

}

char * vulnerability_web_as_string(IDMEFvulnerability_web web) {
	switch(web) {
	case VULNERABILITY_WEB_UNKNOWN:
		return "unknown";
	case VULNERABILITY_WEB_YES:
		return "yes";
	case VULNERABILITY_WEB_NO:
		return "no";
	}

	return 0;

}


IDMEFresult_severity get_result_severity(char *p)
 {
	if (!strcmp(p, "info")) {
		return RESULT_SEVERITY_INFO;
	}
        else
  	if (!strcmp(p, "low")) {
		return RESULT_SEVERITY_LOW;
	}
	else
	if (!strcmp(p, "medium")) {
		return RESULT_SEVERITY_MEDIUM;
	}
	else
	if (!strcmp(p, "high")) {
		return RESULT_SEVERITY_HIGH;
	}

	return -1;

 }
 
char * result_severity_as_string(IDMEFresult_severity severity) {
	switch(severity) {
	case RESULT_SEVERITY_INFO:
		return "info";
	case RESULT_SEVERITY_LOW:
		return "low";
	case RESULT_SEVERITY_MEDIUM:
		return "medium";
	case RESULT_SEVERITY_HIGH:
		return "high";
	}

	return 0;
}

IDMEFresult_type get_result_type(char *p) {

	if (!strcmp(p, "automatic")) {
		return RESULT_TYPE_AUTOMATIC;
	}
	else
	if (!strcmp(p, "manual")) {
		return RESULT_TYPE_MANUAL;
	}

	return -1;

}

char * result_type_as_string(IDMEFresult_type type) {
	switch(type) {
	case RESULT_TYPE_AUTOMATIC:
		return "automatic";
	case RESULT_TYPE_MANUAL:
		return "manual";
	}

	return 0;

}

#endif /* ASSET */
