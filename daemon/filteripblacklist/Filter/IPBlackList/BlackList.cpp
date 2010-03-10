/*
 * BlackList.cpp
 *
 */
#include "Filter/IPBlackList/BlackList.hpp"

namespace Filter
{
namespace IPBlackList
{

bool BlackList::operator[](const IP &ip) const
{
  IPList::const_iterator it=bl_.find(ip);
  return it!=bl_.end();
}

void BlackList::swap(BlackList &other)
{
  bl_.swap( other.bl_ );
}

} // namespace IPBlackList
} // namespace Filter
