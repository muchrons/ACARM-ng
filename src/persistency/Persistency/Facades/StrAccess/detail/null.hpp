/*
 * null.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NULL_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NULL_HPP_FILE

/* public header */

#include <cstdlib>

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

inline const char *null(void)
{
  return "<NULL>";
} // null()

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
