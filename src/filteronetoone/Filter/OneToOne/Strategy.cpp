/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Filter/OneToOne/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherHosts.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;

namespace Filter
{
namespace OneToOne
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout):
  Filter::Simple::Strategy<Data>( TypeName("onetoone"), name, timeout)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName(ConfigConsts::defaultInputTypeName) );
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  const Algo::GatherHosts<ConstHostPtrNN> gh(n);
  if( gh.getSourceHosts().size()!=1 ||
      gh.getTargetHosts().size()!=1    )
    return Data();

  // ok - entry is interesting
  assert( gh.getSourceHosts().size()==1 );
  assert( gh.getTargetHosts().size()==1 );
  return Data( (*gh.getSourceHosts().begin())->getIP(),
               (*gh.getTargetHosts().begin())->getIP() );
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  return thisEntry.t_.isValid_;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // thisEntry and otherEntry must containt the same hosts here, so they
  // can be used interchangebly.
  assert( thisEntry.t_.from_==otherEntry.t_.from_ );
  assert( thisEntry.t_.to_  ==otherEntry.t_.to_   );
  stringstream ss;
  ss << "[one2one] multiple attacks from "
     << thisEntry.t_.from_ << " host on "
     << otherEntry.t_.to_ << " host";
  return ss.str();
}


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );

  if( thisEntry.t_.from_!=otherEntry.t_.from_ )
    return false;
  if( thisEntry.t_.to_  !=otherEntry.t_.to_   )
    return false;
  // if all entries are equal, we can correlate this one
  return true;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &/*otherEntry*/,
                                      const Node       /*newNode*/) const
{
  return thisEntry.t_;
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace OneToOne
} // namespace Filter
