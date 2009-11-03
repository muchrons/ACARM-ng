/*
 * Node.cpp
 *
 */
#include <algorithm>
#include <sstream>
#include <iostream>                         

#include "XML/Node.hpp"

using namespace std;


namespace XML
{

namespace
{
struct NodeComparator
{
  NodeComparator(const string &name):
    name_(name)
  {
  }

  bool operator()(const Node &n1) const
  {
    return n1.getName()==name_;
  }

  const string &name_;
}; // struct NodeComparator
} // unnamed namespace

const Node &Node::getChild(const std::string &name) const
{
  TNodesList::const_iterator it=getChild(name, children_.begin() );
  // check result
  if( it==children_.end() ) // no such element?
    throw Exception("XML::Node::getChild(): "
                    "node '" + name + "' not found");
  return *it;
}

Node::TNodesList::const_iterator Node::getChild(
    const std::string                &name,
    Node::TNodesList::const_iterator  from) const
{
  const NodeComparator nc(name);
  return find_if(from, children_.end(), nc);
}

string Node::getValuesString(void) const
{
  stringstream ss;
  for(TValuesList::const_iterator it=values_.begin();
      it!=values_.end();
      ++it)
  {
    ss<<it->get();
  }
  return ss.str();
}

} // namespace XML
