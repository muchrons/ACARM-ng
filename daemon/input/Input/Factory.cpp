#include "ConfigIO/InputConfig.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Input/Factory.hpp"

using namespace ConfigIO;

namespace Input
{

InputsCollection create(Core::Types::AlertsFifo &output)
{
  InputsCollection out;

  // create each input and configure it
  const InputsConfigCollection &inputs=Singleton::get()->inputsConfig();
  for(InputsConfigCollection::const_iterator iit=inputs.begin();
      iit!=inputs.end(); ++iit)
  {
    // create each and every element
    const Factory::FactoryTypeName   &name   =iit->getType();
    const Factory::Options           &options=iit->getOptions();
    Factory::FactoryPtr               tmp    =Factory::create(name, options);
    ReaderPtrNN                       reader( tmp.release() );
    Persistency::IO::ConnectionPtrNN  conn( Persistency::IO::create().release() );
    InterfacePtrNN                    iface( new Interface(reader, conn, output) );
    // add input to collection
    out.push_back(iface);
  }

  // return collection
  return out;
}

} // namespace Input
