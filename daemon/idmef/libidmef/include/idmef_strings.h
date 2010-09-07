/*
Author: Adam C. Migus <adam@migus.org>

Copyright (c) 2004 Adam C. Migus.  All rights reserved

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_strings.h,v 1.5 2009/02/15 14:15:04 alcapone Exp $
*/

#ifndef _IDMEFXML_STRINGS_H
#define _IDMEFXML_STRINGS_H 1

/*
 * Convert between enumeration types and strings.
 */
char * node_category_as_string(IDMEFnode_category category);
IDMEFnode_category get_node_category(char *p);

char * address_category_as_string(IDMEFaddress_category category);
IDMEFaddress_category get_address_category(char *p);

IDMEFspoofed get_spoofed(char *p);
char * spoofed_as_string(IDMEFspoofed spoofed);

IDMEFdecoy get_decoy(char *p);
char * decoy_as_string(IDMEFdecoy decoy);

IDMEFuser_category get_user_category(char *p);
char * user_category_as_string(IDMEFuser_category category);

IDMEFuserid_type get_userid_type(char *p);
char * userid_type_as_string(IDMEFuserid_type type);

IDMEForigin get_classification_origin(char *p);
char * classification_origin_as_string(IDMEForigin origin);

IDMEFadditionaldata_type get_additionaldata_type(char *p);
char * additionaldata_type_as_string(IDMEFadditionaldata_type type);

IDMEFimpact_severity get_impact_severity(char *p);
char * impact_severity_as_string(IDMEFimpact_severity severity);

IDMEFimpact_completion get_impact_completion(char *p);
char * impact_completion_as_string(IDMEFimpact_completion completion);

IDMEFimpact_type get_impact_type(char *p);
char * impact_type_as_string(IDMEFimpact_type type);

IDMEFaction_category get_action_category(char *p);
char * action_category_as_string(IDMEFaction_category category);

IDMEFconfidence_rating get_confidence_rating(char *p);
char * confidence_rating_as_string(IDMEFconfidence_rating rating);

IDMEFfile_category get_file_category(char *p);
char * file_category_as_string(IDMEFfile_category category);

IDMEFfstype get_file_fstype(char *p);
char * file_fstype_as_string(IDMEFfstype fstype);

IDMEFlinkage_category get_linkage_category(char *p);
char * linkage_category_as_string(IDMEFlinkage_category category);

/* added for draft 12 */
IDMEForigin get_reference_origin(char *p);
char * reference_origin_as_string(IDMEForigin origin);

IDMEFchecksum_algorithm get_checksum_algorithm(char *p);
char * checksum_algorithm_as_string(IDMEFchecksum_algorithm algorithm);

#ifdef IDMEF_ASSET
IDMEFruleaction get_rule_action(char *p);
char * rule_action_as_string(IDMEFruleaction action);

IDMEFlog get_rule_log(char *p);
char * rule_log_as_string(IDMEFlog log);

IDMEFarch get_os_arch(char *p);
char * os_arch_as_string(IDMEFarch arch);

IDMEFarch get_os_family(char *p);
char * os_family_as_string(IDMEFfamily family);

IDMEFvulnerability_exploitable get_vulnerability_exploitable(char *p);
char * vulnerability_exploitable_as_string(IDMEFvulnerability_exploitable exploitable);

IDMEFvulnerability_location get_vulnerability_location(char *p);
char * vulnerability_location_as_string(IDMEFvulnerability_location location);

IDMEFvulnerability_attacktype get_vulnerability_attacktype(char *p);
char * vulnerability_attacktype_as_string(IDMEFvulnerability_attacktype
attacktype);

IDMEFvulnerability_attackimpact get_vulnerability_attackimpact(char *p);
char * vulnerability_attackimpact_as_string(IDMEFvulnerability_attackimpact attackimpact);

IDMEFvulnerability_grade get_vulnerability_grade(char *p);
char * vulnerability_grade_as_string(IDMEFvulnerability_grade grade);

IDMEFvulnerability_web get_vulnerability_web(char *p);
char * vulnerability_web_as_string(IDMEFvulnerability_web web);

IDMEFresult_severity get_result_severity(char *p);
char * result_severity_as_string(IDMEFresult_severity severity);

IDMEFresult_type get_result_type(char *p);
char * result_type_as_string(IDMEFresult_type type);

#endif /* ASSET */
#endif /* _IDMEFXML_STRINGS_H */
