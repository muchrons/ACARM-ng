/* compile with
   gcc -I/usr/include/libidmef -Wall `pkg-config --cflags --libs libxml-2.0` `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs libidmef` -o example3 example3.c
*/
#include <stdio.h>
#include <unistd.h>

#include <libidmef/idmef_types.h>
#include <libidmef/idmef_global.h>
#include <libidmef/idmef_create.h>
#include <libidmef/idmef_list.h>

int
main(int argc, char **argv)
{
	IDMEFPtr idmefp, idmefpalert, idmefpsrc, idmefptgt, idmefpclass,idmefp_msg_tmp;

	idmefpalert = newAlert(
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
			), NULL
		);

	idmefpsrc= newSource(
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
			);
                        
	idmefptgt= newTarget(
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
			);
                        
	idmefpclass= newClassification(
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
			);

        idmefp = newIDMEF_Message(
			newIDMEF_Message_version("1.0", NULL),
                        NULL
                       );

        /* add global attributes to IDMEF-Message */
        idmefp_msg_tmp = newIDMEF_Message_xmlns ("urn:iana:xml:ns:idmef", NULL);
        libidmef_list_set_attribute(idmefp, idmefp_msg_tmp);
        
        /* add alert to IDMEF-Message */
        libidmef_list_add_element(idmefp, idmefpalert);
        
        /* add Source and Target to Alert */
        libidmef_list_add_element(idmefpsrc, idmefptgt);
        
        /* add Classification to Alert */
        libidmef_list_add_element(idmefpalert, idmefpclass);

	/* dump the XML message */
	xmlDocFormatDump (stdout, idmefp->IDMEFXmlDocPtr, 1);

	/* and free the generated IDMEFPtr's recursively */
	libidmef_idmefp_destroy (idmefpalert ,1);
        libidmef_idmefp_destroy (idmefpsrc ,1);
        libidmef_idmefp_destroy (idmefptgt ,1);
        libidmef_idmefp_destroy (idmefpclass ,1);
        libidmef_idmefp_destroy (idmefp, 1);

	return (0);
}
