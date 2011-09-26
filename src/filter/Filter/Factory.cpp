/*
 * Factory.cpp
 *
 */
#include <cassert>

#include "Filter/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace ConfigIO;
using namespace Core::Types::Proc;

namespace Filter
{

FiltersCollection create(Core::Types::SignedNodesFifo &outputQueue)
{
  const FiltersConfigCollection        &c=Singleton::get()->filtersConfig();
  FiltersCollection                     out;
  const ConfigIO::Preprocessor::Config  ppCfg;

  for(FiltersConfigCollection::const_iterator it=c.begin(); it!=c.end(); ++it)
  {
    Factory::FactoryPtr wrapper( Factory::create( it->getType(), it->getOptions() ) );
    assert(wrapper.get()!=NULL);
    assert(it->getPreprocessorConfig()==NULL && "preprocessor for filters is NOT implemented");
    ProcessorPtrNN      filter( new Processor(outputQueue, wrapper->ptr_, ppCfg) );
    out.push_back(filter);
  }

  return out;
}

} // namespace Filter
