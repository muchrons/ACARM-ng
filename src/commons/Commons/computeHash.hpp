/*
 * computeHash.hpp
 *
 */
#ifndef INCLUDE_COMMONS_COMPUTEHASH_HPP_FILE
#define INCLUDE_COMMONS_COMPUTEHASH_HPP_FILE

/* public header */

#include <string>

#include "Commons/Exception.hpp"

namespace Commons
{
/** \brief computes (SHA1) hash of a given string.
 *  \param in input string.
 *  \return computed hash of a given string.
 */
std::string computeHash(const std::string &in);
} // namespace Commons

#endif
