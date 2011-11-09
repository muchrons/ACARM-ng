/*
 * ConfigFileReader.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/ConfigFileReader.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
  template<typename TException>
  void ensureThrow(const char *msg, const char *path) const
  {
    try
    {
      ConfigFileReader cfr(path);
      tut::fail(msg);
    }
    catch(const TException&)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/ConfigFileReader");
} // unnamed namespace


namespace tut
{

// try opening some exising XML file, with read permission
template<>
template<>
void testObj::test<1>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/basic_file.xml");
}

// open basic file and test content
template<>
template<>
void testObj::test<2>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/basic_file.xml");
  const XML::Node r=cfr.getTree().getRoot();
  ensure_equals("invalid root's name", r.getName(), "some_root");

  const XML::Node::TNodesList &children1=r.getChildrenList();
  ensure_equals("invalid root's children count", children1.size(), 1);

  const XML::Node &child11=(*children1.begin());
  ensure_equals("invalid name of child 1.1", child11.getName(), "some_node");
  const XML::Node::TNodesList &children2=child11.getChildrenList();

  ensure_equals("invalid child's name", children2.size(), 2);
  XML::Node::TNodesList::const_iterator it=children2.begin();
  ensure_equals("invalid child 2.1 name", it->getName(), "key");
  ensure_equals("invalid child 2.1 value", it->getValuesString(), "question");
  ++it;
  ensure_equals("invalid child 2.2 name", it->getName(), "value");
  ensure_equals("invalid child 2.2 value", it->getValuesString(), "42");
}

// open non-xml file
template<>
template<>
void testObj::test<3>(void)
{
  const char *msg="opening non-xml file didn't failed";
  ensureThrow<Exception>(msg, "testdata/cfg_file_expand/non-xml-file.xml");
}

// check for exception when file cannot be read
template<>
template<>
void testObj::test<4>(void)
{
  const char *msg="opening file without permissions didn't caused an error";
  ensureThrow<Exception>(msg, "/etc/shadow");
}

// check for exception when file does not exist
template<>
template<>
void testObj::test<5>(void)
{
  const char *msg="opening non-exisitng file didn't caused an error";
  ensureThrow<Exception>(msg, "/file/that/does/not/exist.txt");
}

// check for exception when file does not exist, but can be created
template<>
template<>
void testObj::test<6>(void)
{
  const char *msg="opening non-exisitng file didn't caused an error";
  ensureThrow<Exception>(msg, "_file_that_does_not_exist_but_can_be_created.txt");
}

} // namespace tut
