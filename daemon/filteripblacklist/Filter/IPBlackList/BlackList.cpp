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

} // namespace IPBlackList
} // namespace Filter
