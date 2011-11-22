/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "System/ignore.hpp"
#include "Filter/EventChain/Strategy.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;

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


Timestamp getTs(const Strategy::Node &n)
{
  if( n->getAlert()->getDetectionTime()!=NULL )
    return *n->getAlert()->getDetectionTime();
  return n->getAlert()->getCreationTime();
} // getTs()


typedef const Strategy::NodeEntry*                 ConstNodeEntryPtr;
typedef pair<ConstNodeEntryPtr, ConstNodeEntryPtr> ConstNodeEntryPtrPair;

ConstNodeEntryPtrPair fromToOrder(const Strategy::NodeEntry &entry1, const Strategy::NodeEntry &entry2)
{
  // find from/to chains
  ConstNodeEntryPtr from=NULL;
  ConstNodeEntryPtr to  =NULL;

  // check for chain entry1->entry2
  if( hasCommonIP(entry1.t_.endIPs_, entry2.t_.beginIPs_) )
    if( entry1.t_.endTs_<=entry2.t_.beginTs_ )
    {
      from=&entry1;
      to  =&entry2;
    }

  // check for chain entry2->entry1
  if( hasCommonIP(entry2.t_.endIPs_, entry1.t_.beginIPs_) )
    if( entry2.t_.endTs_<=entry1.t_.beginTs_ )
    {
      from=&entry2;
      to  =&entry1;
    }

  // ensure assignments were valid
  assert(from!=NULL);
  assert(to  !=NULL);
  assert( hasCommonIP(from->t_.endIPs_, to->t_.beginIPs_) );
  assert( from->t_.endTs_<=to->t_.beginTs_ );

  return ConstNodeEntryPtrPair(from, to);
} // fromToOrder()

} // unnamed namespace


Strategy::Params::Params(unsigned int timeout, double priDelta_):
  timeout_(timeout),
  priDelta_(priDelta_)
{
}

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Params &params):
  Filter::Simple::Strategy<Data>( TypeName("eventchain"), name, params.timeout_),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  // accept events only from input
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName(ConfigConsts::defaultInputTypeName) );
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  const Timestamp ts( getTs(n) );
  Data            d;
  Algo::GatherIPs ips(n);
  typedef Algo::GatherIPs::IPSet IPSet;
  d.beginIPs_=boost::shared_ptr<IPSet>( new IPSet( ips.getSourceIPs() ) );
  d.beginTs_ =ts;
  d.endIPs_  =boost::shared_ptr<IPSet>( new IPSet( ips.getTargetIPs() ) );
  d.endTs_   =ts;
  d.len_     =1;
  return d;
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  // if there is source/destination address(s) missing - skip entry
  if( thisEntry.t_.beginIPs_->size()==0u || thisEntry.t_.endIPs_->size()==0u )
    return false;
  // skip entries entries where source and destination IPs are the same
  if( thisEntry.t_.beginIPs_->size()==thisEntry.t_.endIPs_->size() )
  {
    // check if all elements are identical
    for(Algo::GatherIPs::IPSet::const_iterator it1=thisEntry.t_.beginIPs_->begin(), it2=thisEntry.t_.endIPs_->begin();
        it1!=thisEntry.t_.beginIPs_->end(); ++it1, ++it2)
    {
      if(it1->first!=it2->first)
        return true;
    }
    // alle elements identical - not an interesting entry
    return false;
  } // if(equal_sizes)
  // accept this entry
  return false;
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
  if( !isEntryInteresting(thisEntry)  )
    return false;
  if( !isEntryInteresting(otherEntry) )
    return false;

  // check for chain thisEntry->otherEntry
  if( hasCommonIP(thisEntry.t_.endIPs_, otherEntry.t_.beginIPs_) )
    if( thisEntry.t_.endTs_<=otherEntry.t_.beginTs_ )
      return true;
  // check for chain otherEntry->thisEntry
  if( hasCommonIP(otherEntry.t_.endIPs_, thisEntry.t_.beginIPs_) )
    if( otherEntry.t_.endTs_<=thisEntry.t_.beginTs_ )
      return true;
  // no correlation found - stop search
  return false;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       newNode) const
{
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // find from/to chains
  ConstNodeEntryPtrPair p   =fromToOrder(thisEntry, otherEntry);
  const NodeEntry      *from=p.first;
  const NodeEntry      *to  =p.second;
  // sanity check
  assert( isEntryInteresting(*from) );
  assert( isEntryInteresting(*to)   );

  // log some info
  LOGMSG_DEBUG_S(log_)<<"connecting from: '"<<from->node_->getMetaAlert()->getName().get()<<"' to: '"
                      <<to->node_->getMetaAlert()->getName().get()<<"' as: '"
                      <<newNode->getMetaAlert()->getName().get();

  // make output
  Data d;
  d.beginIPs_=from->t_.beginIPs_;
  d.beginTs_ =from->t_.beginTs_;
  d.endIPs_  =to->t_.endIPs_;
  d.endTs_   =to->t_.endTs_;
  d.len_     =from->t_.len_ + to->t_.len_;
  assert(d.len_!=0u);
  assert(d.len_>=2u);
  assert(d.beginTs_<=d.endTs_);
  System::ignore(newNode);
  // NOTE: newly correlated entry (i.e. data) may NOT be interesting!
  return d;
}

void Strategy::postProcessNode(Node &n, Filter::BackendFacade &bf) const
{
  // update severity delta by a given ammount
  bf.updateSeverityDelta(n, params_.priDelta_);
}

void Strategy::postProcessNode(NodeEntry &entry, const NodeEntry &added, BackendFacade &/*bf*/) const
{
  assert( isEntryInteresting(entry) );
  assert( isEntryInteresting(added) );
  // find from/to chains
  ConstNodeEntryPtrPair p   =fromToOrder(entry, added);
  const NodeEntry      *from=p.first;
  const NodeEntry      *to  =p.second;
  // sanity check
  assert( isEntryInteresting(*from) );
  assert( isEntryInteresting(*to)   );

  entry.t_.beginIPs_=from->t_.beginIPs_;
  entry.t_.beginTs_ =from->t_.beginTs_;
  entry.t_.endIPs_  =to->t_.endIPs_;
  entry.t_.endTs_   =to->t_.endTs_;
  entry.t_.len_     =from->t_.len_ + to->t_.len_;
  assert(entry.t_.len_!=0u);
  assert(entry.t_.len_>=2u);
  assert(entry.t_.beginTs_<=entry.t_.endTs_);
}

} // namespace EventChain
} // namespace Filter
