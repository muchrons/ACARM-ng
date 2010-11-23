/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Filter/EventChain/Strategy.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace EventChain
{

namespace
{
bool hasCommonIP(const Data::SharedIPSet &s1, const Data::SharedIPSet &s2)
{
  // if one set is not set we can quit stright away
  if( s1.get()==NULL || s2.get()==NULL )
    return false;
  // one set is empty?
  if( s1->size()==0u || s2->size()==0u )
    return false;
  // check for common element
  // NOTE: this is sub-optimal O(n*log(n)) implementation, but it seems to
  //       be just enought taking into consideration how seldom it is called.
  typedef Data::SharedIPSet::value_type::const_iterator ConstIterator;
  for(ConstIterator it1=s1->begin(); it1!=s1->end(); ++it1)
  {
    assert( it1->second.get()>0 );
    if( s2->find(it1->first)!=s2->end() )
    {
      assert( s2->find(it1->first)->second.get() > 0 );
      return true;
    }
  }
  // ok - no common elements found
  return false;
} // hasCommonIP()
} // unnamed namespace


Strategy::Params::Params(unsigned int timeout, double priDelta_):
  timeout_(timeout),
  priDelta_(priDelta_)
{
}

Strategy::Strategy(const std::string &name, const Params &params):
  Filter::Simple::Strategy<Data>("eventchain", name, params.timeout_),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  // accept events only from input
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");       // TODO: magic value
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  // single node does not represent chain
  Data d;
  Algo::GatherIPs ips(n);
  typedef Algo::GatherIPs::IPSet IPSet;
  d.beginIPs_=boost::shared_ptr<IPSet>( new IPSet( ips.getSourceIPs() ) );
  d.endIPs_  =boost::shared_ptr<IPSet>( new IPSet( ips.getTargetIPs() ) );
  d.len_     =1;
  return d;
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  return thisEntry.t_.beginIPs_->size()>0u &&
         thisEntry.t_.endIPs_->size()  >0u;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry /*thisEntry*/,
                                              const NodeEntry /*otherEntry*/) const
{
  return "[eventchain] chain of events";
}


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanity check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );

  // check for chain thisEntry->otherEntry
  if( hasCommonIP(thisEntry.t_.endIPs_, otherEntry.t_.beginIPs_) )
    return true;
  // check for chain otherEntry->thisEntry
  if( hasCommonIP(otherEntry.t_.endIPs_, thisEntry.t_.beginIPs_) )
    return true;
  // no correlation found - stop search
  return false;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       /*newNode*/) const
{
  // find from/to chains
  const NodeEntry *from=NULL;
  const NodeEntry *to  =NULL;
  if( hasCommonIP(thisEntry.t_.endIPs_, otherEntry.t_.beginIPs_) )
  {
    from=&thisEntry;
    to  =&otherEntry;
  }
  else
  {
    // this must be true - otherwise these hosts could not be correlated in a first place
    assert( hasCommonIP(otherEntry.t_.endIPs_, thisEntry.t_.beginIPs_) );
    from=&otherEntry;
    to  =&thisEntry;
  }
  // ensure assignments were valid
  assert(from!=NULL);
  assert(to  !=NULL);
  assert( hasCommonIP(from->t_.endIPs_, to->t_.beginIPs_) );

  // make output
  Data d;
  d.beginIPs_=from->t_.beginIPs_;
  d.endIPs_  =to->t_.endIPs_;
  d.len_     =from->t_.len_ + to->t_.len_;
  assert(d.len_>0u);
  return d;
}

void Strategy::postProcessNode(Node &/*n*/) const
{
  // TODO
  // nothing to be done here
}

} // namespace EventChain
} // namespace Filter
