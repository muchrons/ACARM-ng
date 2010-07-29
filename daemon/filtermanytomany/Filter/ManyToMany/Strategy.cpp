/*
 * Strategy.cpp
 *
 */
#include <algorithm>
#include <cassert>

#include "Filter/ManyToMany/Strategy.hpp"
#include "Algo/GatherIPs.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace ManyToMany
{

Strategy::Params::Params(unsigned int timeout, double similarity):
  timeout_(timeout),
  similarity_(similarity)
{
  if(similarity_<=0)
    throw ExceptionInvalidParameter(SYSTEM_SAVE_LOCATION, "similarity",
                                    "non-positive value does not make sense");
  if(similarity_>1)
    throw ExceptionInvalidParameter(SYSTEM_SAVE_LOCATION, "similarity",
                                    "values above 100% (i.e. 1) are invalid");
}

Strategy::Strategy(const Params &params):
  Filter::Simple::Strategy<Data>("manytomany", params.timeout_),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
  return ecl;
}

Strategy::NodeEntry Strategy::makeThisEntry(const Node n) const
{
  return NodeEntry( n, Data() );
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  const Algo::GatherIPs gip(thisEntry.node_);
  return gip.getSourceIPs().size()>0 &&
         gip.getTargetIPs().size()>0;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry /*thisEntry*/,
                                              const NodeEntry /*otherEntry*/) const
{
  return "[many2many] atacks from multiple hosts on multiple hosts detected";
}


namespace
{
/** \brief helper that determines count of elements of output collection.
 */
class IntersectionOutputIterator
{
public:
  typedef Algo::GatherIPs::IPSet::value_type IPCountPair;

  IntersectionOutputIterator(const Algo::GatherIPs::IPSet &s1, const Algo::GatherIPs::IPSet &s2):
    s1_(&s1),
    s2_(&s2),
    count_(0)
  {
  }

  void operator++(void)
  {
    // ignore this call
  }

  IntersectionOutputIterator &operator*(void)
  {
    // return ourselves to make space for assignment operator.
    return *this;
  }

  inline const IPCountPair &operator=(const IPCountPair &other)
  {
    // add counts from first collection
    assert( s1_!=NULL );
    assert( s1_->find(other.first)!=s1_->end() );
    count_+=s1_->find(other.first)->second.get();

    // add counts from second collection
    assert( s2_!=NULL );
    assert( s2_->find(other.first)!=s2_->end() );
    count_+=s2_->find(other.first)->second.get();

    return other;
  }

  size_t getCount(void) const
  {
    return count_;
  }

private:
  const Algo::GatherIPs::IPSet *s1_;
  const Algo::GatherIPs::IPSet *s2_;
  size_t                        count_;
}; // class IntersectionOutputIterator


/** \brief SWO for elements in map, ordered by the key.
 */
bool ipSWO(const Algo::GatherIPs::IPSet::value_type &left,
           const Algo::GatherIPs::IPSet::value_type &right)
{
  return left.first<right.first;
} // ipSWO()


/** \brief return count of IPs from given set's intersection.
 */
size_t intersectionCount(const Algo::GatherIPs::IPSet &s1, const Algo::GatherIPs::IPSet &s2)
{
  // perform set intersection on source hosts
  IntersectionOutputIterator tmp=set_intersection( s1.begin(), s1.end(),
                                                   s2.begin(), s2.end(),
                                                   IntersectionOutputIterator(s1, s2),
                                                   ipSWO );
  return tmp.getCount();
} // intersectionCount()


/** \brief count all IPs (i.e. number of their instances).
 */
size_t ipsCount(const Algo::GatherIPs::IPSet &s)
{
  size_t cnt=0;
  for(Algo::GatherIPs::IPSet::const_iterator it=s.begin(); it!=s.end(); ++it)
    cnt+=it->second.get();
  return cnt;
} // ipsCount()
} // unnamed namespace


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );

  // compute IPs counts
  const Algo::GatherIPs gipThis (thisEntry.node_);
  const Algo::GatherIPs gipOther(otherEntry.node_);

  // sum IPs counts
  const size_t thisSourceCount =ipsCount( gipThis.getSourceIPs() );
  const size_t thisTargetCount =ipsCount( gipThis.getTargetIPs() );
  const size_t otherSourceCount=ipsCount( gipOther.getSourceIPs() );
  const size_t otherTargetCount=ipsCount( gipOther.getTargetIPs() );
  const size_t totalCount      =thisSourceCount + thisTargetCount +
                                otherSourceCount + otherTargetCount;
  assert( totalCount>0 );

  // compute intersection's count
  const size_t intSourceCount  =intersectionCount( gipThis.getSourceIPs(), gipOther.getSourceIPs() );
  const size_t intTargetCount  =intersectionCount( gipThis.getTargetIPs(), gipOther.getTargetIPs() );
  const size_t totalIntCount   =intSourceCount + intTargetCount;
  assert( totalIntCount<=totalCount );

  // compute similarity level
  const double similarity      =static_cast<double>(totalIntCount)/totalCount;

  // return final response
  assert( 0<params_.similarity_    );
  assert(   params_.similarity_<=1 );
  const bool result=(similarity >= params_.similarity_);
  LOGMSG_DEBUG_S(log_)<<"similarity: "<<similarity*100
                      <<"%; threshold: "<<params_.similarity_*100
                      <<"%; decision: "<<(result?"":"not ")<<"correlating";
  return result;
}

} // namespace ManyToMany
} // namespace Filter
