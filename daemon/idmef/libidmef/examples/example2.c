/* compile with
** gcc -I/usr/include/libidmef -Wall `pkg-config --cflags --libs libxml-2.0` `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs libidmef` -o example2 example2.c
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>
#include <libidmef/idmef_types.h>
#include <libidmef/idmef_global.h>
#include <libidmef/idmef_create.h>
#include <libidmef/idmef_parser.h>
#include <libidmef/idmef_message.h>

int
main(int argc, char **argv)
 {
  IDMEFmessage *msg;
  gchar buffer[] = 
"<?xml version=\"1.0\"?>\n"
"<!DOCTYPE IDMEF-Message PUBLIC \"-//IETF//DTD RFC XXXX IDMEF v1.0//EN\" \"/usr/share/idmef-message.dtd\">\n"
"<IDMEF-Message version=\"1.0\" xmlns=\"urn:iana:xml:ns:idmef\"  xml:space=\"default\">\n"
"  <Alert messageid=\"1\">\n"
"    <Analyzer analyzerid=\"a1\" manufacturer=\"Home Inc.\" osversion=\"2.6.10\" ostype=\"linux\">\n"
"      <Node category=\"dns\">\n"
"        <location>Headquarters</location>\n"
"        <name>a1.hq.migus.org</name>\n"
"        <Address ident=\"a0-eth1\" category=\"ipv4-addr\">\n"
"          <address>123.123.234.234</address>\n"
"        </Address>\n"
"      </Node>\n"
"      <Analyzer analyzerid=\"upstream1\">\n"
"        <Node category=\"dns\">\n"
"          <location>Germany</location>\n"
"          <name>upstream1.de.migus.org</name>\n"
"        </Node>\n"
"      </Analyzer>\n"
"    </Analyzer>\n"
"    <CreateTime ntpstamp=\"0xbc72423b.0x00000000\">2000-03-09T15:31:07Z</CreateTime>\n"
"    <Source ident=\"s1\">\n"
"      <Node ident=\"s1-a1\" category=\"dns\">\n"
"        <Address ident=\"a1-eth0\" category=\"ipv4-addr\">\n"
"          <address>1.2.3.4</address>\n"
"        </Address>\n"
"      </Node>\n"
"    </Source>\n"
"    <Target ident=\"t1\">\n"
"      <Node ident=\"t1-a1\" category=\"dns\">\n"
"        <Address ident=\"a1-eth0\" category=\"ipv4-addr\">\n"
"          <address>4.3.2.1</address>\n"
"        </Address>\n"
"      </Node>\n"
"    </Target>\n"
"    <Classification ident=\"class1\" text=\"Portscan detected\">\n"
"      <Reference origin=\"vendor-specific\">\n"
"        <name>portscan</name>\n"
"        <url>http://www.vendor.com/portscan/</url>\n"
"      </Reference>\n"
"    </Classification>\n"
"  </Alert>\n"
"</IDMEF-Message>\n";
  
  printf ("Parsing ../xml/idmef-messages/example1.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example1.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example2.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example2.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example3.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example3.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example4.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example4.xml", NULL, 1);
  if (msg)
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example5.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example5.xml", NULL, 1);
  if (msg)
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example6.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example6.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example7.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example7.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example8.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example8.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example9.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example9.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example10.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example10.xml", NULL, 1);
  if (msg)
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example11.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example11.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example12.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example12.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example13.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example13.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example14.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example14.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example15.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example15.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example16.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example16.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example17.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example17.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example18.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example18.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing ../xml/idmef-messages/example20.xml\n");
  msg = idmef_parse_file ("../xml/idmef-messages/example20.xml", NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }
  printf("\n================================================================================\n");

  printf ("Parsing message in memory:\nlength: %d\n%s\n", strlen (buffer), buffer);
  msg = idmef_parse_memory (buffer, strlen (buffer), NULL, 1);
  if (msg)
   {
    xmlDocFormatDump (stdout, msg->idmefp->IDMEFXmlDocPtr, 1);
    idmef_message_free (msg);
   }

end:  
  return 0;
 }
