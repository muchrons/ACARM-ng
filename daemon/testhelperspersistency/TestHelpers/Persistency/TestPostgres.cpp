/*
 * TestPostgres.cpp
 *
 */
#include "TestHelpers/Persistency/TestPostgres.hpp"
#include "ConfigIO/Singleton.hpp"

namespace
{
const char *configContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <general>"
  "    <url>http://test.url</url>"
  "    <cleanupInterval>42</cleanupInterval>"
  "    <cleanupOlder>41</cleanupOlder>"
  "  </general>"
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
  "      <File name=\"default\">"
  "        <output>acarm-ng.log</output>"
  "      </File>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\" threshold=\"debug\">"
  "    </nodes>"
  "  </logger>"
  ""
  "  <preprocessor/>"
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

TestPostgres::TestPostgres(void):
  TestBase(configContent)
{
}

} // namespace Persistency
} // namespace TestHelpers
