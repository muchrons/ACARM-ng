#include "Filter/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Core::Types::Proc;
using namespace ConfigIO;

namespace Filter
{

FiltersCollection create(Core::Types::UniqueNodesFifo &outputQueue)
{
  const FiltersConfigCollection &c=Singleton::get()->filtersConfig();
  FiltersCollection              out;

  for(FiltersConfigCollection::const_iterator it=c.begin(); it!=c.end(); ++it)
  {
    Processor::InterfaceAutoPtr iface( Factory::create( it->getType(), it->getOptions() ) );
    ProcessorPtrNN filter( new Processor(outputQueue, iface) );
    out.push_back(filter);
  }

  return out;
}

} // namespace Filter
