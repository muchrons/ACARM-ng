/*
 * ParseLoggerNodes.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ParseLoggerNodes.hpp"

using namespace std;
using namespace XML;
using namespace ConfigIO;

namespace
{

const char *xmlOK1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <logger>"
  "    <nodes appender=\"defaultAppender\">"
  "    </nodes>"
  "  </logger>"
  "</acarm_ng>"
  "";

const char *xmlOK2=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <logger>"
  "    <nodes appender=\"app1\">"
  ""
  "      <sub1>"
  "        <sub11 appender=\"appxyz\"/>"
  "      </sub1>"
  ""
  "      <sub2>"
  "        <sub21 appender=\"appxyz\"/>"
  "        <sub22 appender=\"appxyz\"/>"
  "        <sub23>"
  "          <sub231/>"
  "        </sub23>"
  "      </sub2>"
  ""
  "    </nodes>"
  "  </logger>"
  "</acarm_ng>"
  "";

const char *xmlErr1=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <logger>"
  "    <nodes>"         // <-- default appender not defined
  "    </nodes>"
  "  </logger>"
  "</acarm_ng>"
  "";




struct ParseLoggerNodesTestClass
{
  // return copyied, parsed subtree
  Node getNodes(const char *xml) const
  {
    assert(xml!=NULL);
    XMLpp::SaxParser sp;
    const XML::Tree  tree=sp.parse(xml);
    return tree.getRoot().getChild("logger").getChild("nodes");
  }

  // return copyied appenders configs
  ParseLoggerNodes getNodesConfs(const char *xml) const
  {
    const Node             n=getNodes(xml);
    const ParseLoggerNodes pln(n);
    return pln;
  }
};

typedef ParseLoggerNodesTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ParseLoggerNodes");
} // unnamed namespace


namespace tut
{

// self-test
template<>
template<>
void testObj::test<1>(void)
{
  const Node n=getNodes(xmlOK1);
  ensure_equals("invalid node returned by helper", n.getName(), "nodes");
}

// check default node
template<>
template<>
void testObj::test<2>(void)
{
  const ParseLoggerNodes pln=getNodesConfs(xmlOK1);
  ensure_equals("invalid default node name",
                pln.getDefaultNodeConfig().getNodeName(), "");
  ensure_equals("invalid default appender name",
                pln.getDefaultNodeConfig().getAppenderName(),
                "defaultAppender");
}

// check empty nodes tree
template<>
template<>
void testObj::test<3>(void)
{
  const ParseLoggerNodes  pln  =getNodesConfs(xmlOK1);
  const LoggerNodes      &nodes=pln.getNodes();
  ensure("some elements are present in the tree", nodes.begin()==nodes.end() );
}

// test pasring some nodes
template<>
template<>
void testObj::test<4>(void)
{
  const ParseLoggerNodes  pln  =getNodesConfs(xmlOK2);
  const LoggerNodes      &nodes=pln.getNodes();
  ensure("no elements in the tree", nodes.begin()!=nodes.end() );

  // check tree's content
  LoggerNodes::const_iterator it=nodes.begin();

  // sub1
  ensure_equals("invalid node name", it->getNodeName(), "sub1");
  ensure("sub1 node has appender", !it->hasAppender() );
  ++it;

  // sub1.sub11<appxyz>
  ensure("no elemtents in tree after sub1", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub1.sub11");
  ensure("sub1 node does not have appender", it->hasAppender() );
  ensure_equals("inalid appedenr anme for sub1.sub11 node",
                it->getAppenderName(), "appxyz");
  ++it;

  // sub2
  ensure("no elemtents in tree after sub1.sub11", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub2");
  ensure("sub2 node has appender", !it->hasAppender() );
  ++it;

  // sub2.sub21<appxyz>
  ensure("no elemtents in tree after sub2", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub2.sub21");
  ensure("sub2.sub21 node does not have appender", it->hasAppender() );
  ensure_equals("inalid appedenr anme for sub2.sub21 node",
                it->getAppenderName(), "appxyz");
  ++it;

  // sub2.sub22<appxyz>
  ensure("no elemtents in tree after sub2.sub21", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub2.sub22");
  ensure("sub2.sub22 node does not have appender", it->hasAppender() );
  ensure_equals("inalid appedenr anme for sub2.sub22 node",
                it->getAppenderName(), "appxyz");
  ++it;

  // sub2.sub23<appxyz>
  ensure("no elemtents in tree after sub2.sub22", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub2.sub23");
  ensure("sub2.sub23 node has appender", !it->hasAppender() );
  ++it;

  // sub2.sub23<appxyz>.sub231
  ensure("no elemtents in tree after sub2.sub23", it!=nodes.end() );
  ensure_equals("invalid node name", it->getNodeName(), "sub2.sub23.sub231");
  ensure("sub2.sub23.sub231 node has appender", !it->hasAppender() );
  ++it;

  // this houdl be the end
  ensure("some extra elements int the tree detected", it==nodes.end() );
}

// check throw on no default appender
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    getNodesConfs(xmlErr1);
    fail("parsing nodes without default appedner definede didn't throw");
  }
  catch(const XML::Exception &)
  {
    // this is expected.
  }
}

} // namespace tut
