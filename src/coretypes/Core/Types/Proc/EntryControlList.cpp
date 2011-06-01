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

void EntryControlList::add(const TypeName &type)
{
  types_.push_back(type);
}

bool EntryControlList::isAcceptable(const TypeName &type) const
{
  // check if given filter is no our list - if it is, return oposite-to-default value
  for(TypeNamesVector::const_iterator it=types_.begin(); it!=types_.end(); ++it)
    if(*it==type)
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
