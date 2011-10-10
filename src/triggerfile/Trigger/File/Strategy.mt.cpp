/*
 * Strategy.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/File/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger::File;
using namespace Core::Types::Proc;

int main(void)
{
  const Config           cfg( ".", Trigger::Simple::ThresholdConfig("0.0", "0") );
  Strategy               s(InstanceName("myfiletrigger"), cfg);
  Strategy::ChangedNodes nc;
  s.process( TestHelpers::Persistency::makeNewNode(), nc ); // trigger and send
  return 0;
}
