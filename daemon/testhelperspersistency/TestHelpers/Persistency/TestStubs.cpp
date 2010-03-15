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
  "      <File name=\"default\">"
  "        <output>acarm-ng.log</output>"
  "      </File>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\" threshold=\"debug\">"
  "    </nodes>"
  "  </logger>"
  ""
  "  <filters/>"
  ""
  "  <triggers/>"
  ""
  "  <inputs/>"
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
