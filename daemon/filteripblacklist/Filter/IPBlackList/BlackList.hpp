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

/** \brief class repreesnting collection of blacklisted IPs.
 */
class BlackList
{
public:
  /** \brief IP type. */
  typedef Persistency::IPTypes<BlackList>::IP IP;

  /** \brief create instance.
   *  \param begin first iterator to read entry from.
   *  \param end   end iterator.
   */
  template<typename TForwardIterator>
  BlackList(const TForwardIterator begin, const TForwardIterator end)
  {
    for(TForwardIterator it=begin; it!=end; ++it)
      bl_.insert(*it);
  }
  /** \brief checks if igven IP is balcklisted.
   *  \param ip IP to be checked.
   *  \return true if IP is blacklisted, false otherwise.
   */
  bool operator[](const IP &ip) const;

private:
  typedef std::set<IP> IPList;

  IPList bl_;
}; // class BlackList

} // namespace IPBlackList
} // namespace Filter

#endif
