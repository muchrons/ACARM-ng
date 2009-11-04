/*
 * ParseLoggerAppenders.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParseLoggerAppenders.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

const char *xmlOK1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <logger>"
  "    <appenders>"
  ""
  "      <file name=\"fileApp1\">"
  "        <output>somefile.log</output>"
  "      </file>"
  ""
  "      <file name=\"fileApp2\">"
  "        <output>some_other_file.log</output>"
  "      </file>"
  ""
  "      <console name=\"stdout\"/>"
  ""
  "      <multi name=\"multiappender\">"
  "        <appender>fileApp1</appender>"
  "        <appender>stdout</appender>"
  "      </multi>"
  ""
  "    </appenders>"
  "  </logger>"
  "</acarm_ng>"
  "";

const char *xmlErr1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <logger>"
  "    <appenders>"
  ""
  "      <file>"            // <-- appender's name is missing here
  "        <output>somefile.log</output>"
  "      </file>"
  ""
  "    </appenders>"
  "  </logger>"
  "</acarm_ng>"
  "";





struct ParseLoggerAppendersTestClass
{
  // return copyied, parsed subtree
  Node getAppenders(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parse(xml);
    return tree.getRoot().getChild("logger").getChild("appenders");
  }

  // return copyied appenders configs
  LoggerAppenders getAppsConfs(const char *xml) const
  {
    const Node                 n=getAppenders(xml);
    const ParseLoggerAppenders pla(n);
    const LoggerAppenders      apps=pla.getAppenders();
    return apps;
  }
};

typedef ParseLoggerAppendersTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParseLoggerAppenders");
} // unnamed namespace


namespace tut
{

// test to see if helper method getAppenders() works fine (self-test)
template<>
template<>
void testObj::test<1>(void)
{
  const Node n=getAppenders(xmlOK1);
  ensure_equals("invalid node returned by helper", n.getName(), "appenders");
}

// try parsing and see if all appenders and options are present
template<>
template<>
void testObj::test<2>(void)
{
  const LoggerAppenders           apps=getAppsConfs(xmlOK1);
  LoggerAppenders::const_iterator it  =apps.begin();

  // check first
  ensure("colection is empty", it!=apps.end() );
  ensure_equals("invalid name of appender 1", it->getName(), "fileApp1");
  ensure_equals("invalid type of appender 1", it->getType(), "file");
  ensure_equals("'output' option is missing for appender 1",
                (*it)["output"].at(0), "somefile.log");
  ensure_equals("invalid options count in appender 1",
                (*it)["output"].size(), 1);
  ++it;

  // check second
  ensure("colection has just one element", it!=apps.end() );
  ensure_equals("invalid name of appender 2", it->getName(), "fileApp2");
  ensure_equals("invalid type of appender 2", it->getType(), "file");
  ensure_equals("'output' option is missing for appender 2",
                (*it)["output"].at(0), "some_other_file.log");
  ensure_equals("invalid options count in appender 2",
                (*it)["output"].size(), 1);
  ++it;

  // check third
  ensure("colection has only two elements", it!=apps.end() );
  ensure_equals("invalid name of appender 3", it->getName(), "stdout");
  ensure_equals("invalid type of appender 3", it->getType(), "console");
  ++it;

  // check fourth
  ensure("colection has only three elements", it!=apps.end() );
  ensure_equals("invalid name of appender 4", it->getName(), "multiappender");
  ensure_equals("invalid type of appender 4", it->getType(), "multi");
  ensure_equals("appender option 1 is invalid for appender 4",
                (*it)["appender"].at(0), "fileApp1");
  ensure_equals("appender option 2 is invalid for appender 4",
                (*it)["appender"].at(1), "stdout");
  ensure_equals("invalid options count in appender 4",
                (*it)["appender"].size(), 2);
  ++it;
}

// test appender without name
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    getAppsConfs(xmlErr1);
    fail("parsing XML didn't failed for appender without name");
  }
  catch(const XML::Exception&)
  {
    // this is expected
  }
}

} // namespace tut
