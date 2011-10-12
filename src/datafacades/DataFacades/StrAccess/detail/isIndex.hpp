/*
 * isIndex.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_DETAIL_ISINDEX_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_DETAIL_ISINDEX_HPP_FILE

/* public header */

#include <string>

#include "System/NoInstance.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

/** \brief checks if given string is a valid index.
 *  \param str string to be checked.
 *  \return true, if string is an index, false otherwise.
 */
bool isIndex(const std::string &str);

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
