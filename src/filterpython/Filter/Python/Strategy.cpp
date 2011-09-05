/*
 * Strategy.cpp
 *
 */
#include <cassert>

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
    env.importModule("filter");
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
  ecl.add( TypeName("*input*") );   // TODO: magic value
  return ecl;
}

DataPtr Strategy::makeThisEntryUserData(const Node /*n*/) const
{
  // TODO
  return DataPtr();
}

bool Strategy::isEntryInteresting(const NodeEntry /*thisEntry*/) const
{
  // TODO
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(const NodeEntry thisEntry,
                                                        const NodeEntry otherEntry) const
{
  // TODO
  // thisEntry and otherEntry must containt the same meta-alert name
  assert( canCorrelate(thisEntry, otherEntry) );
  stringstream ss;
  ss << "[python]";
  return ss.str();
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // TODO
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  return false;
}

DataPtr Strategy::makeUserDataForNewNode(const NodeEntry &/*thisEntry*/,
                                         const NodeEntry &/*otherEntry*/,
                                         const Node       /*newNode*/) const
{
  // TODO
  return DataPtr();
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
