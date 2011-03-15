/*
 * computeHash.hpp
 *
 */
#ifndef INCLUDE_COMMONS_COMPUTEHASH_HPP_FILE
#define INCLUDE_COMMONS_COMPUTEHASH_HPP_FILE

/* public header */

#include <string>

namespace Commons
{
  /** \brief computes (SHA1) hash of a given string.
   *  \param in input string.
   */
  std::string computeHash(const std::string &in);
}  // namespace Commons

#endif
