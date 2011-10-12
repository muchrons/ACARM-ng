/*
 * isIndex.cpp
 *
 */
#include <ctype.h>

#include "DataFacades/StrAccess/detail/isIndex.hpp"


namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

bool isIndex(const std::string &str)
{
  if(str.length()<1)
    return false;
  for(std::string::const_iterator it=str.begin(); it!=str.end(); ++it)
    if(!isdigit(*it))
      return false;
  return true;
}

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency
