/*
 * CheckHosts.hpp
 *
 */
#ifndef INCLUDE_FILTER_HOSTCOMON_CHECKHOSTS_HPP_FILE
#define INCLUDE_FILTER_HOSTCOMON_CHECKHOSTS_HPP_FILE

#include "Filter/HostCommon/Strategy.hpp"

namespace Filter
{
namespace HostCommon
{

/** \brief helper object to check if host is unique.
 */
struct CheckHosts
{
  /** \brief create object.
   */
  explicit CheckHosts(const Strategy *s):
    noMatch_(false),
    s_(s)
  {
    assert(s_!=NULL);   // this is only internal implementation.
  }

  /** \brief checks given leaf for uniqueness of Host entries.
   *  \param node node (i.e. leaf) to be checked.
   */
  void operator()(Persistency::GraphNodePtrNN node)
  {
    // assert is fine here, since this is implementation internal.
    assert( node->isLeaf() );
    // if we already know there is not match, just skip the call
    if(noMatch_)
      return;
    // check each and every reported host
    assert(s_!=NULL);   // this is only internal implementation.
    const Persistency::Alert::Hosts &hs=s_->getHostsArray(node);
    for(Persistency::Alert::Hosts::const_iterator it=hs.begin();
        it!=hs.end(); ++it)
    {
      if( out_.get()==NULL )
        out_=*it;                 // first entry is always a match
      else
        if( out_->getIP()!=(*it)->getIP() )
        {
          // there is no unique host entry.
          out_.reset();
          noMatch_=true;
          break;
        }
    } // for(reported hosts)
  }

  Persistency::HostPtr  out_;       ///< output object

private:
  bool                  noMatch_;   ///< helper flag
  const Strategy       *s_;         ///< strategy implementation
}; // struct CheckHosts

} // namespace HostCommon
} // namespace Filter

#endif
