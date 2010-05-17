/*
 * graphNodePrinteout.cpp
 *
 */
#include <string>
#include <iostream>

#include "Persistency/GraphNode.hpp"
#include "TestHelpers/Persistency/graphNodePrintout.hpp"

using namespace std;

namespace TestHelpers
{
namespace Persistency
{

namespace
{

void graphNodePrintoutImpl(::Persistency::GraphNodePtr n, const string &prefix)
{
  // print this node's name
  cout << prefix << "> " << n->getMetaAlert()->getName().get()
       << "  (" << static_cast<void*>( n.get() ) << ")" << endl;

  // if it has childrent, print them too
  if( n->isLeaf()==false )
    for(::Persistency::GraphNode::iterator it=n->begin(); it!=n->end(); ++it)
      graphNodePrintoutImpl(*it, prefix+"  ");
} // graphNodePrintoutImpl()

} // unnamed namespace



void graphNodePrintout(const std::vector< ::Persistency::GraphNodePtr > &v)
{
  for(std::vector< ::Persistency::GraphNodePtr >::const_iterator it=v.begin(); it!=v.end(); ++it)
  {
    cout << "-------------- " << static_cast<void*>( it->get() ) << " --------------" << endl;
    graphNodePrintout(*it);
  }
} // graphNodePrintout()

void graphNodePrintout(::Persistency::GraphNodePtr n)
{
  graphNodePrintoutImpl(n, "");
} // graphNodePrintout()

} // namespace Persistency
} // namespace TestHelpers
