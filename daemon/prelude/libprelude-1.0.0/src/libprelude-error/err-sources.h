/* Output of mkstrtable.awk.  DO NOT EDIT.  */

/* err-sources.h - List of error sources and their description.
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
  gettext_noop ("Unspecified source") "\0"
  gettext_noop ("prelude-io") "\0"
  gettext_noop ("prelude-message") "\0"
  gettext_noop ("prelude-failover") "\0"
  gettext_noop ("prelude-client") "\0"
  gettext_noop ("prelude-client-profile") "\0"
  gettext_noop ("prelude-extract") "\0"
  gettext_noop ("prelude-connection") "\0"
  gettext_noop ("prelude-connection-pool") "\0"
  gettext_noop ("config-file") "\0"
  gettext_noop ("prelude-string") "\0"
  gettext_noop ("idmef-tree-wrap") "\0"
  gettext_noop ("idmef-path") "\0"
  gettext_noop ("idmef-type") "\0"
  gettext_noop ("idmef-message-read") "\0"
  gettext_noop ("idmef-criteria") "\0"
  gettext_noop ("idmef-value-type") "\0"
  gettext_noop ("libpreludedb") "\0"
  gettext_noop ("Unknown source");

static const int msgidx[] =
  {
    0,
    19,
    30,
    46,
    63,
    78,
    101,
    117,
    136,
    160,
    172,
    187,
    203,
    214,
    225,
    244,
    259,
    276,
    289
  };

#define msgidxof(code) (0 ? -1 \
  : ((code >= 0) && (code <= 16)) ? (code - 0) \
  : ((code >= 255) && (code <= 255)) ? (code - 238) \
  : 256 - 238)
