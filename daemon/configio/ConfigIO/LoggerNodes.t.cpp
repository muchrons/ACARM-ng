/*
 * LoggerNodes.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/LoggerNodes.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct LoggerNodesTestClass
{
  LoggerNodesTestClass(void)
  {
  }

  LoggerNodeConfig mkNodeCfg(const char *name,
                             const char *appName=NULL,
                             const char *thr="debug") const
  {
    if(appName!=NULL)
      return LoggerNodeConfig(name, appName, thr);
    return LoggerNodeConfig(name, OptionalString(), OptionalString() );
  }

  LoggerNodes::Nodes ns_;
};

typedef LoggerNodesTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/LoggerNodes");
} // unnamed namespace


namespace tut
{

// test empty collection
template<>
template<>
void testObj::test<1>(void)
{
  const LoggerNodes ln(ns_);
  ensure("non-zero elements in colelction", ln.begin()==ln.end() );
}

// test adding 3 unique elements
template<>
template<>
void testObj::test<2>(void)
{
  ns_.push_back( mkNodeCfg("some.name.1") );
  ns_.push_back( mkNodeCfg("some.name.2", "app1") );
  ns_.push_back( mkNodeCfg("some.name.3", "app2", "info") );
  const LoggerNodes ln(ns_);

  // check content
  LoggerNodes::const_iterator it=ln.begin();

  ensure("zero elements in colelction", it!=ln.end() );
  ensure_equals("invalid first element", it->getNodeName(), "some.name.1");
  ensure("element 1 reports it has appender", !it->hasAppender() );
  ++it;

  ensure("just one elements in colelction", it!=ln.end() );
  ensure_equals("invalid second element", it->getNodeName(), "some.name.2");
  ensure("element 2 reports no appender", it->hasAppender() );
  ++it;

  ensure("just two elements in colelction", it!=ln.end() );
  ensure_equals("invalid third element", it->getNodeName(), "some.name.3");
  ensure("element 3 reports no appender",  it->hasAppender()  );
  ensure("element 3 reports no threshold", it->hasThreshold() );
  ++it;

  ensure("more than tow elements in collection", it==ln.end() );
}

// test throw on buplicated entry
template<>
template<>
void testObj::test<3>(void)
{
  ns_.push_back( mkNodeCfg("some.name.1") );
  ns_.push_back( mkNodeCfg("some.name.2", "app1") );
  ns_.push_back( mkNodeCfg("some.name.1") );
  try
  {
    LoggerNodes ln(ns_);
    fail("LoggerNodes() didn't throw on duplicated entries");
  }
  catch(const ExceptionDuplicatedNodeName&)
  {
    // this is expected
  }
}

} // namespace tut
