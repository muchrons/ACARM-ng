/*
 * TestStubs.cpp
 *
 */
#include "TestHelpers/Persistency/TestStubs.hpp"

namespace
{
const char *configContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <persistency>"
  "    <type>stubs</type>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <file name=\"default\">"
  "        <output>loggerfile.log</output>"
  "      </file>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\">"
  "    </nodes>"
  "  </logger>"
  ""
  "</acarm_ng>"
  ;
}

namespace TestHelpers
{
namespace Persistency
{

TestStubs::TestStubs(void):
  TestBase(configContent)
{
}

} // namespace Persistency
} // namespace TestHelpers
