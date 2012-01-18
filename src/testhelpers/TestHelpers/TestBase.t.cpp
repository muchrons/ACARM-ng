/*
 * TestBase.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <stdlib.h>

#include "TestHelpers/TestBase.hpp"

using namespace TestHelpers;

namespace
{
const char *someTestConfigContent=
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<acarm_ng>"
      ""
      "  <general>"
      "    <url>http://test.url</url>"
      "    <cleanupInterval>42</cleanupInterval>"
      "    <cleanupOlder>41</cleanupOlder>"
      "    <pluginsDir>/some/dir/name</pluginsDir>"
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

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/TestBase");
} // unnamed namespace


namespace tut
{

// check for persistency section
template<>
template<>
void testObj::test<1>(void)
{
  const TestBase tb(someTestConfigContent);
  ensure_equals("no persistency section", system("grep -q '<persistency>' test_base_config_file.xml"), 0);
}

// check for logger section
template<>
template<>
void testObj::test<2>(void)
{
  const TestBase tb(someTestConfigContent);
  ensure_equals("no logger section", system("grep -q '<logger>' test_base_config_file.xml"), 0);
}

} // namespace tut
