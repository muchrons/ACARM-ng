#include <cassert>

#include "Trigger/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Core::Types::Proc;
using namespace ConfigIO;

namespace Trigger
{

TriggersCollection create(Core::Types::SignedNodesFifo &outputQueue)
{
  const TriggersConfigCollection &c=Singleton::get()->triggersConfig();
  TriggersCollection              out;

  for(TriggersConfigCollection::const_iterator it=c.begin(); it!=c.end(); ++it)
  {
    Factory::FactoryPtr wrapper( Factory::create( it->getType(), it->getOptions() ) );
    assert(wrapper.get()!=NULL);
    ProcessorPtrNN      trigger( new Processor(outputQueue, wrapper->ptr_) );
    out.push_back(trigger);
  }

  return out;
}

} // namespace Trigger
