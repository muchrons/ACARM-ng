/*
 * isIndex.cpp
 *
 */
#include <ctype.h>

#include "DataFacades/StrAccess/detail/isIndex.hpp"


namespace DataFacades
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
} // namespace DataFacades
