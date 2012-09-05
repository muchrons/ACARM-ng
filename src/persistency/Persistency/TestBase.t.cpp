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
  "  <general>"
  "    <url>http://test.url</url>"
  "    <cleanupInterval>42</cleanupInterval>"
  "    <cleanupOlder>41</cleanupOlder>"
  "    <pluginsDir>/some/dir/name</pluginsDir>"
  "    <alertsLimit>1983</alertsLimit>"
  "  </general>"
  ""
  "  <persistency>"
  "    <type>persistencytest</type>"
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
} // unnamed namespace

namespace Persistency
{

TestBase::TestBase(void):
  TestHelpers::TestBase(defaultConfigContent)
{
}

} // namespace Persistency
