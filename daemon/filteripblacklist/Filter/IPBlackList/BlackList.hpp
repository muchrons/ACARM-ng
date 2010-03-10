/*
 * BlackList.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_BLACKLIST_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_BLACKLIST_HPP_FILE

#include <set>

#include "Persistency/IPTypes.hpp"

namespace Filter
{
namespace IPBlackList
{

class BlackList
{
public:
  typedef Persistency::IPTypes<BlackList>::IP IP;

  template<typename TForwardIterator>
  BlackList(const TForwardIterator begin, const TForwardIterator end)
  {
    for(TForwardIterator it=begin; it!=end; ++it)
      bl_.insert(*it);
  }

  bool operator[](const IP &ip) const;

private:
  typedef std::set<IP> IPList;

  IPList bl_;
}; // class BlackList

} // namespace IPBlackList
} // namespace Filter

#endif
