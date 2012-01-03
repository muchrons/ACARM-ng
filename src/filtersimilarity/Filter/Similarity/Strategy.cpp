/*
 * Strategy.cpp
 *
 */
#include <algorithm>
#include <sstream>
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Logger/Logger.hpp"
#include "Filter/Similarity/Strategy.hpp"
#include "Algo/Diff/compare.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;

namespace Filter
{
namespace Similarity
{

Strategy::Params::Params(const unsigned int timeout, const double similarity):
  timeout_(timeout),
  similarity_(similarity)
{
  if(similarity_<0)
    throw ExceptionInvalidParameter(SYSTEM_SAVE_LOCATION, "similairty", "value must be non-negative");
  if(similarity_>1)
    throw ExceptionInvalidParameter(SYSTEM_SAVE_LOCATION, "similairty", "value must be less than or equal to 1");
}

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Params params):
  Filter::Simple::Strategy<Data>( TypeName("similarity"), name, params.timeout()),
  params_(params)
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
  return Data( n->getAlert() );
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  return thisEntry.node_->isLeaf();
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(const NodeEntry thisEntry,
                                                        const NodeEntry otherEntry) const
{
  stringstream ss;
  const std::string thisName( thisEntry.node_->getMetaAlert()->getName().get() );
  const std::string otherName( otherEntry.node_->getMetaAlert()->getName().get() );
  ss << "[similarity] '" << thisName;
  if(thisName!=otherName)
    ss << "' and '" << otherName;
  ss << "'";
  return ss.str();
}

namespace
{
typedef Algo::Diff::Similarity Sim;

Sim computeSimilarity(const ConstAlertPtr a1, const ConstAlertPtr a2)
{
  if(a1.get()==NULL)
    return 0;
  if(a2.get()==NULL)
    return 0;

  return Algo::Diff::compare(*a1, *a2);
} // computeSimilarity()

ConstAlertPtr convert(const ConstAlertPtr &in)
{
  return in;
} // convert()
ConstAlertPtr convert(const ConstAlertPtrNN &in)
{
  return in.shared_ptr();
} // convert()

Sim computeSimilarity(const Strategy::NodeEntry &e1, const Strategy::NodeEntry &e2)
{
  const Sim s11=computeSimilarity( convert(e1.t_.first_ ), convert(e2.t_.first_ ) );
  const Sim s12=computeSimilarity( convert(e1.t_.first_ ), convert(e2.t_.second_) );
  const Sim s21=computeSimilarity( convert(e1.t_.second_), convert(e2.t_.first_ ) );
  const Sim s22=computeSimilarity( convert(e1.t_.second_), convert(e2.t_.second_) );
  return max( max( s11.get(), s12.get() ), max( s21.get(), s22.get() ) );
} // computeSimilarity()
} // unnamed namespace

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  const Sim s=computeSimilarity(thisEntry, otherEntry);
  LOGMSG_DEBUG_S(log_) << "similairty between " << thisEntry.node_->getMetaAlert()->getID().get() << " and "
                       << otherEntry.node_->getMetaAlert()->getID().get() << " is " << s.get()*100 << "%"
                       << " (threshold is " << params_.similarity()*100 << "%)";
  return s.get()>=params_.similarity();
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       /*newNode*/) const
{
  assert( thisEntry.node_->isLeaf() || otherEntry.node_->isLeaf() );
  if( !thisEntry.node_->isLeaf() )
    return Data(thisEntry.t_);
  if( !otherEntry.node_->isLeaf() )
    return Data(otherEntry.t_);
  // ok - correlating nw alert
  return Data( thisEntry.node_->getAlert(), otherEntry.node_->getAlert().shared_ptr() );
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace Similarity
} // namespace Filter
