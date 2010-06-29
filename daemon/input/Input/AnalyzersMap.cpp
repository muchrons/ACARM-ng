/*
 * AnalyzersMap.cpp
 *
 */
#include "Input/AnalyzersMap.hpp"

namespace Input
{

Persistency::AnalyzerPtrNN::SharedPtr AnalyzersMap::get(const std::string &id) const
{
  InputIDMap::const_iterator it=map_.find(id);
  if( it==map_.end() )
    return Persistency::AnalyzerPtrNN::SharedPtr(); // NULL
  return it->second;
}

void AnalyzersMap::add(const std::string &id, Persistency::AnalyzerPtrNN analyzer)
{
  // do not allow owerwrites (it means some sort of error)
  if( get(id).get()!=NULL )
    throw ExceptionAddingAlreadyExistingKey(SYSTEM_SAVE_LOCATION, id, analyzer);
  // add to collection
  map_.insert( InputIDMap::value_type(id, analyzer) );
  assert( get(id)==analyzer && "entry not added" );
}

} // namespace Input
