/* compile with
** gcc -I/usr/include/libidmef -Wall -g -O0 `pkg-config --cflags --libs libxml-2.0` `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs libidmef` -o test test.c
*/
#include <stdio.h>
#include <unistd.h>

#include <libidmef/idmef_types.h>
#include <libidmef/idmef_global.h>
#include <libidmef/idmef_create.h>

int
main(int argc, char **argv)
{
	IDMEFPtr idmefp;

	idmefp = newIDMEF_Message(
		   newIDMEF_Message_version("1.0", NULL),
		   newAlert(
		       newAlert_messageid("1", NULL), NULL), NULL);


#if 0                        
			newAnalyzer(
				newAnalyzer_analyzerid("a1"),
				newNode(
					newNode_category("dns"),
					newNode_location(
						"Headquarters", NULL
					),
					newNode_name(
						"a1.hq.migus.org", NULL
					), NULL
				), NULL	
			),
			newCreateTime(
				"2000-03-09T15:31:07Z",
				newCreateTime_ntpstamp(
					"0xbc72423b.0x00000000"
				), NULL
			),
			newSource(
				newSource_ident("s1"),
				newNode(
					newNode_ident("s1-a1"),
					newNode_category("dns"),
					newAddress(
						newAddress_ident("a1-eth0"),
						newAddress_category(
							"ipv4-addr"
						),
						newAddress_address(
							"1.2.3.4", NULL
						), NULL
					), NULL
				), NULL
			),
			newTarget(
				newTarget_ident("t1"),
				newNode(
					newNode_ident("t1-a1"),
					newNode_category("dns"),
					newAddress(
						newAddress_ident("a1-eth0"),
						newAddress_category(
							"ipv4-addr"
						),
						newAddress_address(
							"4.3.2.1", NULL
						), NULL
					), NULL
				), NULL
			),
			newClassification(
                        	newClassification_ident ("class1"),
                                newClassification_text ("Portscan detected"),
                        	newReference(
				        newReference_origin("vendor-specific"),
				        newReference_name(
					        "portscan", NULL
				        ),
				        newReference_url(
					"http://www.vendor.com/portscan/", NULL
					), NULL
				), NULL
			), NULL
		), NULL
	);
#endif

	xmlDocFormatDump(stdout, idmefp->IDMEFXmlDocPtr, 1);
	libidmef_idmefp_destroy (idmefp, 1);
        
	return (0);
}
