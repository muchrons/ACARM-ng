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
			newAlert_messageid("1", NULL),
			newAnalyzer(
				newAnalyzer_analyzerid("a1", NULL),
				newNode(
					newNode_category("dns", NULL),
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
					"0xbc72423b.0x00000000", NULL
				), NULL
			),
			newSource(
				newSource_ident("s1", NULL),
				newNode(
					newNode_ident("s1-a1", NULL),
					newNode_category("dns", NULL),
					newAddress(
						newAddress_ident("a1-eth0", NULL),
						newAddress_category(
							"ipv4-addr", NULL
						),
						newAddress_address(
							"1.2.3.4", NULL
						), NULL
					), NULL
				), NULL
			),
			newTarget(
				newTarget_ident("t1", NULL),
				newNode(
					newNode_ident("t1-a1", NULL),
					newNode_category("dns", NULL),
					newAddress(
						newAddress_ident("a1-eth0", NULL),
						newAddress_category(
							"ipv4-addr", NULL
						),
						newAddress_address(
							"4.3.2.1", NULL
						), NULL
					), NULL
				), NULL
			),
			newClassification(
                        	newClassification_ident ("class1", NULL),
                                newClassification_text ("Portscan detected", NULL),
                        	newReference(
				        newReference_origin("vendor-specific", NULL),
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

	/* dump the XML message */
	xmlDocFormatDump(stdout, idmefp->IDMEFXmlDocPtr, 1);

	/* and free it recursively */
	libidmef_idmefp_destroy (idmefp, 1);

	return (0);
}
