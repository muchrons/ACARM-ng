/*
 * TestPostgres.cpp
 *
 */
#include "TestHelpers/Persistency/TestPostgres.hpp"

namespace
{
const char *configContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <persistency>"
  "    <type>postgres</type>"
  "    <host>localhost</host>"
  "    <port>5432</port>"
  "    <dbname>acarm_ng_test</dbname>"
  "    <user>acarm-ng-daemon</user>"
  "    <pass>test.daemon</pass>"
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

TestPostgres::TestPostgres(void):
  TestBase(configContent)
{
}

} // namespace Persistency
} // namespace TestHelpers
