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
  "  <general>"
  "    <url>http://test.url</url>"
  "    <cleanupInterval>42</cleanupInterval>"
  "    <cleanupOlder>41</cleanupOlder>"
  "    <pluginsDir>testdata/plugins</pluginsDir>"
  "    <memoryLimit>1983</memoryLimit>"
  "  </general>"
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

TestStubs::TestStubs(void):
  TestBase(configContent)
{
}

} // namespace Persistency
} // namespace TestHelpers
