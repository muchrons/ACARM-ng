/*
 * NodeName.cpp
 *
 */
#include <cctype>
#include <cstring>

#include "Logger/NodeName.hpp"

namespace Logger
{

NodeName::NodeName(const char *name):
  name_( validate(name) )
{
}

NodeName::NodeName(const NodeName &parentNode, const char *surfix):
  name_( parentNode.get() + '.' + validate(surfix) )
{
}

const char *NodeName::validate(const char *name)
{
  // sanity check
  if(name==NULL)
    throw ExceptionNullParameter(SYSTEM_SAVE_LOCATION, "name");

  // check for lower case, numbers and '.' - nothing more is allowed
  const int len=strlen(name);
  for(int i=0; i<len; ++i)
    if( !islower(name[i]) && !isdigit(name[i]) && name[i]!='.' )
      throw ExceptionInvalidNodeName(SYSTEM_SAVE_LOCATION, name);

  return name;
}

} // namespace Logger
