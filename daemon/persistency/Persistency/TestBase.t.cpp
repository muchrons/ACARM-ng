/*
 * TestBase.t.cpp
 *
 */
#include "Persistency/TestBase.t.hpp"

namespace
{
const char *defaultConfigContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <persistency>"
  "    <type>persistencytest</type>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <file name=\"default\">"
  "        <output>loggerfile.log</output>"
  "      </file>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\" threshold=\"debug\">"
  "    </nodes>"
  "  </logger>"
  ""
  "</acarm_ng>"
  ;
} // unnamed namespace

namespace Persistency
{

TestBase::TestBase(void):
  TestHelpers::TestBase(defaultConfigContent)
{
}

} // namespace Persistency
