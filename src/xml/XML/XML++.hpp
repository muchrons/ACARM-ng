/*
 * XML++.hpp
 *
 * wrapper for XML++ files, that disables some of the warning.
 *
 */
#ifndef INCLUDE_XML_XMLPP_HPP_FILE
#define INCLUDE_XML_XMLPP_HPP_FILE

/* public header */

#ifdef __ICC
#pragma warning disable 1292
#endif

#include <libxml++/libxml++.h>

#ifdef __ICC
#pragma warning enable 1292
#endif

#endif
