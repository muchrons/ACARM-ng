/*
 * AttributesList.cpp
 *
 */
#include <algorithm>

#include "XML/AttributesList.hpp"

using namespace std;

namespace XML
{

namespace
{
class AttributeNameCmp
{
public:
  AttributeNameCmp(const Attribute &attr):
    attr_(attr)
  {
  }

  bool operator()(const Attribute &attr) const
  {
    return attr_.getName()==attr.getName();
  }

private:
  const Attribute attr_;
}; // struct AttributeNameCmp
} // unnamed namespace


void AttributesList::add(const Attribute &attr)
{
  {
    // first check if there is no duplicate element.
    const AttributeNameCmp anc(attr);
    const_iterator it=find_if(begin(), end(), anc);
    if( it!=end() )
      throw Exception("XML::AttributeNameCmp::add(): "
                      "duplicate entry cannot be added;"
                      "name: " + attr.getName() );
  }
  // if there is no such element, add it to collection.
  list_.push_back(attr);
}

AttributesList::const_iterator AttributesList::find(const std::string &name) const
{
  const AttributeNameCmp anc( Attribute(name, "") );
  return find_if(begin(), end(), anc);
}

const Attribute &AttributesList::getAttribute(const std::string &name) const
{
  const const_iterator it=find(name);
  if( it==end() )
    throw Exception("XML::getAttribute(): "
                    "no attribute named '" + name + "'");
  return *it;
}

} // namespace XML
