/*
 * graphNodePrintout.mt.cpp
 *
 */
#include <iostream>

#include "TestHelpers/Persistency/graphNodePrintout.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace TestHelpers::Persistency;

int main(void)
{
  {
    cout<<"single graph printout:"<<endl;
    graphNodePrintout( makeNewTree1() );
  }

  cout<<endl;

  {
    cout<<"graph vector printout:"<<endl;
    std::vector< ::Persistency::GraphNodePtr > v;
    v.push_back( makeNewTree1() );
    v.push_back( makeNewTree2() );
    graphNodePrintout(v);
  }

  return 0;
}
