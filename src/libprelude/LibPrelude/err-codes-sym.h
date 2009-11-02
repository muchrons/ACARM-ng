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
  "PRELUDE_ERROR_NO_ERROR" "\0"
  "PRELUDE_ERROR_GENERIC" "\0"
  "PRELUDE_ERROR_TLS" "\0"
  "PRELUDE_ERROR_INVAL_LENGTH" "\0"
  "PRELUDE_ERROR_INVAL_MESSAGE" "\0"
  "PRELUDE_ERROR_PROTOCOL_VERSION" "\0"
  "PRELUDE_ERROR_EOF" "\0"
  "PRELUDE_ERROR_PROFILE" "\0"
  "PRELUDE_ERROR_TLS_AUTH_REJECTED" "\0"
  "PRELUDE_ERROR_INVAL_INT8" "\0"
  "PRELUDE_ERROR_INVAL_INT16" "\0"
  "PRELUDE_ERROR_INVAL_INT32" "\0"
  "PRELUDE_ERROR_INVAL_INT64" "\0"
  "PRELUDE_ERROR_INVAL_FLOAT" "\0"
  "PRELUDE_ERROR_INVAL_CHAR" "\0"
  "PRELUDE_ERROR_INVAL_IDMEF_TIME" "\0"
  "PRELUDE_ERROR_CONNECTION_STRING" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_UNKNOWN" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_COPY_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_CLONE_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_COMPARE_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_READ_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_WRITE_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_COMPARE_MISMATCH" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_PARSE" "\0"
  "PRELUDE_ERROR_IDMEF_CLASS_UNKNOWN" "\0"
  "PRELUDE_ERROR_IDMEF_CLASS_UNKNOWN_CHILD" "\0"
  "PRELUDE_ERROR_IDMEF_CLASS_UNKNOWN_NAME" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_LENGTH" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_DEPTH" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_PARENT_ROOT" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_NTH" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_MISS_INDEX" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_INTEGRITY" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_INDEX_RESERVED" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_INDEX_FORBIDDEN" "\0"
  "PRELUDE_ERROR_IDMEF_PATH_INDEX_UNDEFINED" "\0"
  "PRELUDE_ERROR_IDMEF_TREE_INDEX_UNDEFINED" "\0"
  "PRELUDE_ERROR_IDMEF_UNKNOWN_ENUM_STRING" "\0"
  "PRELUDE_ERROR_IDMEF_UNKNOWN_TAG" "\0"
  "PRELUDE_ERROR_DAEMONIZE_LOCK_HELD" "\0"
  "PRELUDE_ERROR_PLUGIN_LTDL_INIT" "\0"
  "PRELUDE_ERROR_STRING_NOT_NULL_TERMINATED" "\0"
  "PRELUDE_ERROR_CANT_RESOLVE" "\0"
  "PRELUDE_ERROR_UNKNOWN_PERMISSION_TYPE" "\0"
  "PRELUDE_ERROR_UNKNOWN_PERMISSION_BIT" "\0"
  "PRELUDE_ERROR_IDMEF_CRITERIA_PARSE" "\0"
  "PRELUDE_ERROR_IDMEF_CRITERION_UNSUPPORTED_OPERATOR" "\0"
  "PRELUDE_ERROR_IDMEF_CRITERION_INVALID_REGEX" "\0"
  "PRELUDE_ERROR_TLS_WARNING_ALERT" "\0"
  "PRELUDE_ERROR_TLS_FATAL_ALERT" "\0"
  "PRELUDE_ERROR_IDMEF_CLASS_CHILD_NOT_CLASS" "\0"
  "PRELUDE_ERROR_ASSERTION" "\0"
  "PRELUDE_ERROR_IDMEF_VALUE_TYPE_REF_UNAVAILABLE" "\0"
  "PRELUDE_ERROR_UNKNOWN_ERRNO" "\0"
  "PRELUDE_ERROR_CODE_DIM";

static const int msgidx[] =
  {
    0,
    23,
    45,
    63,
    90,
    118,
    149,
    167,
    189,
    221,
    246,
    272,
    298,
    324,
    350,
    375,
    406,
    438,
    477,
    525,
    574,
    625,
    673,
    722,
    770,
    807,
    841,
    881,
    920,
    952,
    983,
    1020,
    1049,
    1085,
    1120,
    1160,
    1201,
    1242,
    1283,
    1323,
    1355,
    1389,
    1420,
    1461,
    1488,
    1526,
    1563,
    1598,
    1649,
    1693,
    1725,
    1755,
    1797,
    1821,
    1868,
    1896
  };

#define msgidxof(code) (0 ? -1 \
  : ((code >= 0) && (code <= 6)) ? (code - 0) \
  : ((code >= 8) && (code <= 8)) ? (code - 1) \
  : ((code >= 16) && (code <= 53)) ? (code - 8) \
  : ((code >= 55) && (code <= 62)) ? (code - 9) \
  : ((code >= 16382) && (code <= 16382)) ? (code - 16328) \
  : 16383 - 16328)
/* public header */
