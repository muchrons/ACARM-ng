/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/readTextFile.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/Strategy.hpp"

using namespace std;
using namespace Core::Types::Proc;

namespace Trigger
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
  PythonAPI::Environment env;
  env.importModule("persistency");
  env.importModule("trigger");
  env.run(script.get());
  // get the result
  boost::shared_ptr<Base> ptr=env.var< boost::shared_ptr<Base> >("derived");
  env.run("derived=None");
  return BasePtrNN(ptr);
} // derivedFromScript()
} // unnamed namespace


Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Trigger::Simple::Strategy(TypeName("python"), name, cfg.getThresholdConfig() ),
  impl_( derivedFromScript(cfg.getScriptPath()) )
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // forward this to python's implementation
  assert(impl_.get()!=NULL);
  impl_->trigger(n);
}

} // namespace Python
} // namespace Trigger
