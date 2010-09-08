/*
 * Writer.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "RFCIO/XML/Writer.hpp"

using namespace std;
using namespace RFCIO::XML;

namespace
{

struct TestClass
{
  TestClass(void)
  {
  }

  std::stringstream ss_;
  xmlpp::Document   doc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/XLM/Writer");
} // unnamed namespace


namespace tut
{

// test wrtiting empty XML
template<>
template<>
void testObj::test<1>(void)
{
  Writer w(doc_);
  w.write(ss_);
  ensure_equals("invalid XML", ss_.str(), "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
}

// test writing some XML
template<>
template<>
void testObj::test<2>(void)
{
  doc_.create_root_node("hello_test");
  Writer w(doc_);
  w.write(ss_);
  ensure_equals("invalid XML", ss_.str(), "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<hello_test/>\n");
}

} // namespace tut
