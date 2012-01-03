/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Commons/Filesystem/readTextFile.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/ExceptionHandle.hpp"
#include "Filter/Python/Strategy.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;

namespace Filter
{
namespace Python
{

namespace
{
BasePtrNN derivedFromScript(const Config::Path &path)
{
  // read script from file
  boost::shared_array<char> script=Commons::Filesystem::readTextFile(path);
  assert(script.get()!=NULL);
  // run this script
  try
  {
    // build derived object's instance
    PythonAPI::Environment env;
    env.importModule("persistency");
    env.importModule("filterapi");
    env.run(script.get());
    // get the result
    BasePtr ptr=env.var<BasePtr>("derived");
    env.run("derived=None");
    return BasePtrNN(ptr);
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
  // -----
  assert(!"code should never reach here");
  throw std::logic_error("code should never reach here");
} // derivedFromScript()
} // unnamed namespace



Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Filter::Simple::Strategy<DataPtr>( TypeName("python"), name, cfg.getTimeout()),
  base_( derivedFromScript(cfg.getScriptPath()) )
{
  assert(base_.get()!=NULL);
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName(ConfigConsts::defaultInputTypeName) );
  return ecl;
}

DataPtr Strategy::makeThisEntryUserData(const Node n) const
{
  assert(base_.get()!=NULL);
  return base_->makeThisEntryUserData(n);
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  assert(base_.get()!=NULL);
  return base_->isEntryInteresting(thisEntry.node_, thisEntry.t_);
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(const NodeEntry thisEntry,
                                                        const NodeEntry otherEntry) const
{
  // sanity check
  assert( canCorrelate(thisEntry, otherEntry) );
  // processing
  assert(base_.get()!=NULL);
  return base_->getMetaAlertName(thisEntry.node_, thisEntry.t_, otherEntry.node_, otherEntry.t_);
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // processing
  assert(base_.get()!=NULL);
  return base_->canCorrelate(thisEntry.node_, thisEntry.t_, otherEntry.node_, otherEntry.t_);
}

DataPtr Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                         const NodeEntry &otherEntry,
                                         const Node       newNode) const
{
  assert(base_.get()!=NULL);
  return base_->makeUserDataForNewNode(thisEntry.node_, thisEntry.t_, otherEntry.node_, otherEntry.t_, newNode);
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace Python
} // namespace Filter
