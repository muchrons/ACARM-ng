/*
 * libidmefHeaders.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEFIO_LIBIDMEFHEADERS_HPP_FILE
#define INCLUDE_RFCIO_IDMEFIO_LIBIDMEFHEADERS_HPP_FILE

//
// this header is required since libidmef does not properly handle
// inclusion from C++. beside that inclusion order DOES matter...
//

extern "C"
{
#include "libidmef/idmef_types.h"
#include "libidmef/idmef_create.h"
#include "libidmef/idmef_global.h"
#include "libidmef/idmef_list.h"
#include "libidmef/idmef_message.h"
#include "libidmef/idmef_parser.h"
#include "libidmef/idmef_parser_helpers.h"
#include "libidmef/idmef_strings.h"
} // extern "C"

#endif
