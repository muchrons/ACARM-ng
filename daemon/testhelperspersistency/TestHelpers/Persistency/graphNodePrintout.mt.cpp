/*
 * graphNodePrintout.mt.cpp
 *
 */
#include "TestHelpers/Persistency/graphNodePrintout.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace TestHelpers::Persistency;

int main(void)
{
  graphNodePrintout( makeNewTree1() );
  return 0;
}
