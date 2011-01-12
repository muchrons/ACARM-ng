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
    Processor::InterfaceAutoPtr iface( Factory::create( it->getType(), it->getOptions() ) );
    ProcessorPtrNN trigger( new Processor(outputQueue, iface) );
    out.push_back(trigger);
  }

  return out;
}

} // namespace Trigger
