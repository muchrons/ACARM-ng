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
       << "  (" << static_cast<const void*>( n.get() ) << ")" << endl;

  // if it has childrent, print them too
  if( n->isLeaf()==false )
    for(::Persistency::GraphNode::iterator it=n->begin(); it!=n->end(); ++it)
      graphNodePrintoutImpl(*it, prefix+"  ");
} // graphNodePrintoutImpl()

template<typename TCol>
void graphNodePrintoutCollecitonImpl(const TCol &c)
{
  for(typename TCol::const_iterator it=c.begin(); it!=c.end(); ++it)
  {
    cout << "-------------- " << static_cast<const void*>( it->get() ) << " --------------" << endl;
    graphNodePrintout(*it);
  }
} // graphNodePrintoutCollecitonImpl()

} // unnamed namespace



void graphNodePrintout(const std::vector< ::Persistency::GraphNodePtr > &v)
{
  graphNodePrintoutCollecitonImpl(v);
} // graphNodePrintout()

void graphNodePrintout(const std::vector< ::Persistency::GraphNodePtrNN > &v)
{
  graphNodePrintoutCollecitonImpl(v);
} // graphNodePrintout()

void graphNodePrintout(::Persistency::GraphNodePtr n)
{
  graphNodePrintoutImpl(n, "");
} // graphNodePrintout()

void graphNodePrintout(::Persistency::GraphNodePtrNN n)
{
  graphNodePrintoutImpl(n, "");
} // graphNodePrintout()

} // namespace Persistency
} // namespace TestHelpers
