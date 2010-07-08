/*
 * EntryControlList.cpp
 *
 */
#include "Core/Types/Proc/EntryControlList.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{

EntryControlList EntryControlList::createDefaultAccept(void)
{
  return EntryControlList(true);
}

EntryControlList EntryControlList::createDefaultReject(void)
{
  return EntryControlList(false);
}

void EntryControlList::add(const FilterName &filterName)
{
  filterNames_.push_back(filterName);
}

bool EntryControlList::isAcceptable(const FilterName &filterName) const
{
  // check if given filter is no our list - if it is, return oposite-to-default value
  for(FilterNamesVector::const_iterator it=filterNames_.begin(); it!=filterNames_.end(); ++it)
    if(*it==filterName)
      return !defaultPolicy_;
  // if entry is not on the list, just give the default value
  return defaultPolicy_;
}

EntryControlList::EntryControlList(bool acceptByDefault):
  defaultPolicy_(acceptByDefault)
{
}

} // namespace Proc
} // namespace Types
} // namespace Core
