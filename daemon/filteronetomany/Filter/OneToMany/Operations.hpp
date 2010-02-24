/*
 * Operations.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOMANY_OPERATIONS_HPP_FILE
#define INCLUDE_FILTER_ONETOMANY_OPERATIONS_HPP_FILE

#include <sstream>
#include <cassert>

#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/HostCommon/Strategy.hpp"

namespace Filter
{
namespace OneToMany
{

/** \brief helpers oprations implementation.
 */
struct Operations
{
private:
  typedef Filter::HostCommon::Strategy Types;

  struct CheckHosts
  {
    CheckHosts(void):
      noMatch_(false)
    {
    }

    void operator()(Persistency::GraphNodePtrNN node)
    {
      assert( node->isLeaf() );
      // if we already know there is not match, just skip the call
      if(noMatch_)
        return;
      // check each and every reported host
      const Persistency::Alert::ReportedHosts &hs=
                                node->getAlert()->getReportedSourceHosts();
      for(Persistency::Alert::ReportedHosts::const_iterator it=hs.begin();
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

    Persistency::HostPtr out_;

  private:
    bool                 noMatch_;
  }; // struct CheckHosts

public:
  /** \brief computes reoprted host and downs it to one.
   *  \param node node to compute them for.
   *  \return host pointer or NULL if not correlation has been found.
   */
  static Persistency::HostPtr getReportedHost(const Types::Node node)
  {
    return Algo::forEachUniqueLeaf( node, CheckHosts() ).out_;
  }

  /** \brief generates name for meta alert, based on given host name.
   *  \param h host to generate name from.
   *  \return name for this alert.
   */
  static Persistency::MetaAlert::Name getMetaAlertName(const Persistency::HostPtrNN h)
  {
    std::stringstream ss;
    ss << "Multiple attacks from host " << h->getIP();
    if( h->getName().get()!=NULL )
      ss << " (" << h->getName().get() << ")";
    return ss.str();
  }
}; // class Strategy

} // namespace OneToMany
} // namespace Filter

#endif
