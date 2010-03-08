#include "Filter/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Core::Types::Proc;
using namespace ConfigIO;

namespace Filter
{

FiltersCollection create(void)
{
  const FiltersConfigCollection &c=Singleton::get()->filtersConfig();
  FiltersCollection              out;

  for(FiltersConfigCollection::const_iterator it=c.begin(); it!=c.end(); ++it)
  {
    ProcessorPtrNN filter( Factory::create( it->getType(), it->getOptions() ) );
    out.push_back(filter);
  }

  return out;
}

} // namespace Filter
