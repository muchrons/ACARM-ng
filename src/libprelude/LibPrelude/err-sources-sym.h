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
  "PRELUDE_ERROR_SOURCE_UNKNOWN" "\0"
  "PRELUDE_ERROR_SOURCE_IO" "\0"
  "PRELUDE_ERROR_SOURCE_MESSAGE" "\0"
  "PRELUDE_ERROR_SOURCE_FAILOVER" "\0"
  "PRELUDE_ERROR_SOURCE_CLIENT" "\0"
  "PRELUDE_ERROR_SOURCE_CLIENT_PROFILE" "\0"
  "PRELUDE_ERROR_SOURCE_EXTRACT" "\0"
  "PRELUDE_ERROR_SOURCE_CONNECTION" "\0"
  "PRELUDE_ERROR_SOURCE_CONNECTION_POOL" "\0"
  "PRELUDE_ERROR_SOURCE_CONFIG_ENGINE" "\0"
  "PRELUDE_ERROR_SOURCE_STRING" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_TREE_WRAP" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_PATH" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_TYPE" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_MESSAGE_READ" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_CRITERIA" "\0"
  "PRELUDE_ERROR_SOURCE_IDMEF_VALUE_TYPE" "\0"
  "PRELUDE_ERROR_SOURCE_PRELUDEDB" "\0"
  "PRELUDE_ERROR_SOURCE_DIM";

static const int msgidx[] =
  {
    0,
    29,
    53,
    82,
    112,
    140,
    176,
    205,
    237,
    274,
    309,
    337,
    374,
    406,
    438,
    478,
    514,
    552,
    583
  };

#define msgidxof(code) (0 ? -1 \
  : ((code >= 0) && (code <= 16)) ? (code - 0) \
  : ((code >= 255) && (code <= 255)) ? (code - 238) \
  : 256 - 238)
