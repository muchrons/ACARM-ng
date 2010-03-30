/* Output of mkstrtable.awk.  DO NOT EDIT.  */

/* err-codes.h - List of error codes and their description.
   Copyright (C) 2003, 2004 g10 Code GmbH

   This file is part of libgpg-error.

   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with libgpg-error; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


/* The purpose of this complex string table is to produce
   optimal code with a minimum of relocations.  */

static const char msgstr[] = 
  gettext_noop ("Success") "\0"
  gettext_noop ("Unknown generic error") "\0"
  gettext_noop ("Unknown TLS error") "\0"
  gettext_noop ("Invalid length specified") "\0"
  gettext_noop ("Invalid message") "\0"
  gettext_noop ("Invalid protocol version") "\0"
  gettext_noop ("End of file") "\0"
  gettext_noop ("Client profile does not exist or have wrong permissions") "\0"
  gettext_noop ("Peer rejected authentication") "\0"
  gettext_noop ("Buffer does not contain a 8 bits integer") "\0"
  gettext_noop ("Buffer does not contain a 16 bits integer") "\0"
  gettext_noop ("Buffer does not contain a 32 bits integer") "\0"
  gettext_noop ("Buffer does not contain a 64 bits integer") "\0"
  gettext_noop ("Buffer does not contain a 32 bits float") "\0"
  gettext_noop ("Buffer is not zero terminated") "\0"
  gettext_noop ("Buffer does not contain an IDMEF time type") "\0"
  gettext_noop ("No connection information provided") "\0"
  gettext_noop ("Unknown IDMEF value type") "\0"
  gettext_noop ("Copy operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Clone operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Compare operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Read operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Write operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Can not compare data of different type") "\0"
  gettext_noop ("Could not parse IDMEF value") "\0"
  gettext_noop ("Unknown IDMEF class specified") "\0"
  gettext_noop ("Unknown IDMEF child specified") "\0"
  gettext_noop ("Unknown IDMEF element name") "\0"
  gettext_noop ("IDMEF path name too long") "\0"
  gettext_noop ("IDMEF path maximum depth exceeded") "\0"
  gettext_noop ("Can not make parent from IDMEF root path") "\0"
  gettext_noop ("Specified object index does not exist") "\0"
  gettext_noop ("Listed element require an index to be specified") "\0"
  gettext_noop ("Element integrity failed") "\0"
  gettext_noop ("Provided index value is reserved") "\0"
  gettext_noop ("Element forbid indexing") "\0"
  gettext_noop ("Element index is undefined") "\0"
  gettext_noop ("An index below the one specified is undefined") "\0"
  gettext_noop ("Unknown enumeration string") "\0"
  gettext_noop ("IDMEF message tag is unknown") "\0"
  gettext_noop ("File lock is held by another process") "\0"
  gettext_noop ("Unable to initialize LTDL") "\0"
  gettext_noop ("String is not 'nul' terminated") "\0"
  gettext_noop ("Could not resolve address") "\0"
  gettext_noop ("Specified permission type is unknown") "\0"
  gettext_noop ("Specified permission bit is not known") "\0"
  gettext_noop ("Could not parse IDMEF criteria") "\0"
  gettext_noop ("Unsupported criterion operator") "\0"
  gettext_noop ("Criterion regular expression is not valid") "\0"
  gettext_noop ("TLS layer returned a warning alert") "\0"
  gettext_noop ("TLS layer returned a fatal alert") "\0"
  gettext_noop ("IDMEF child is not a class") "\0"
  gettext_noop ("Critical assertion occurred") "\0"
  gettext_noop ("Reference operation unavailable for specified IDMEF type") "\0"
  gettext_noop ("Unknown system error") "\0"
  gettext_noop ("Unknown error code");

static const int msgidx[] =
  {
    0,
    8,
    30,
    48,
    73,
    89,
    114,
    126,
    182,
    211,
    252,
    294,
    336,
    378,
    418,
    448,
    491,
    526,
    551,
    603,
    656,
    711,
    763,
    816,
    855,
    883,
    913,
    943,
    970,
    995,
    1029,
    1070,
    1108,
    1156,
    1181,
    1214,
    1238,
    1265,
    1311,
    1338,
    1367,
    1404,
    1430,
    1461,
    1487,
    1524,
    1562,
    1593,
    1624,
    1666,
    1701,
    1734,
    1761,
    1789,
    1846,
    1867
  };

#define msgidxof(code) (0 ? -1 \
  : ((code >= 0) && (code <= 6)) ? (code - 0) \
  : ((code >= 8) && (code <= 8)) ? (code - 1) \
  : ((code >= 16) && (code <= 53)) ? (code - 8) \
  : ((code >= 55) && (code <= 62)) ? (code - 9) \
  : ((code >= 16382) && (code <= 16382)) ? (code - 16328) \
  : 16383 - 16328)
