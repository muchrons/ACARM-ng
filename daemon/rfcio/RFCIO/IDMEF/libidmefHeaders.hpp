/*
 * libidmefHeaders.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_LIBIDMEFHEADERS_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_LIBIDMEFHEADERS_HPP_FILE

//
// this header is required since libidmef does not properly handle
// inclusion from C++...
//

extern "C"
{
#include "libidmef/idmef_create.h"
#include "libidmef/idmef_global.h"
#include "libidmef/idmef_list.h"
#include "libidmef/idmef_message.h"
#include "libidmef/idmef_parser.h"
#include "libidmef/idmef_parser_helpers.h"
#include "libidmef/idmef_strings.h"
#include "libidmef/idmef_types.h"
} // extern "C"

#endif
