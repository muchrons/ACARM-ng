/*
 * NodeName.cpp
 *
 */
#include <string>
#include <sstream>
#include <cctype>
#include <cstring>

#include "Logger/NodeName.hpp"

using namespace std;

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

namespace
{
bool isCharValid(const char c)
{
  return isdigit(c) || islower(c) || c=='.';
} // isCharValid()
} // unnamed namespace

std::string NodeName::removeInvalidChars(const std::string &name)
{
  stringstream ss;

  // allow only basic chars - skip the rest
  for(string::const_iterator it=name.begin(); it!=name.end(); ++it)
  {
    char c=*it;
    // lower-case by default
    if( isupper(c) )
      c=tolower(c);
    // check validity
    if( isCharValid(c) )
      ss<<c;
  }

  // return response
  const string &str=ss.str();
  if( str.size()==0 )
    return "unnamed";
  return str;
}

const char *NodeName::validate(const char *name)
{
  // sanity check
  if(name==NULL)
    throw ExceptionNullParameter(SYSTEM_SAVE_LOCATION, "name");

  // check for lower case, numbers and '.' - nothing more is allowed
  const int len=strlen(name);
  for(int i=0; i<len; ++i)
    if( !isCharValid(name[i]) )
      throw ExceptionInvalidNodeName(SYSTEM_SAVE_LOCATION, name);

  return name;
}

} // namespace Logger
