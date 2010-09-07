/*
Author: Sandro Poppi <spoppi@gmx.net>

Copyright (c) 2004 Sandro Poppi.  All rights reserved

A SAX Parser for IDMEF messages

This library is released under the GNU GPL and BSD software licenses.
You may choose to use one or the other, BUT NOT BOTH.  The GNU GPL
license is located in the file named COPYING.  The BSD license is located
in the file named COPYING.BSD.  Please contact us if there are any
questions.

$Id: idmef_parser.h,v 1.3 2005/07/17 11:55:50 alcapone Exp $
*/

#ifndef _IDMEFXML_PARSER_H
#define _IDMEFXML_PARSER_H 1

#include <libidmef/idmef_types.h>

IDMEFmessage* idmef_parse_file   (const char *file, const char *domain,
				  const unsigned int gen_idmefp);
IDMEFmessage* idmef_parse_memory (const char *buffer, int len,
			          const char *domain,
                                  const unsigned int gen_idmefp);

#endif
