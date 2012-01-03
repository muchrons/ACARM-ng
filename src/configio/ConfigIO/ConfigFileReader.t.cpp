/*
 * ConfigFileReader.t.cpp
 *
 */
#include <tut.h>
#include <fstream>
#include <unistd.h>

#include "ConfigConsts/configio.hpp"
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
  ensure_equals("invalid root's name", r.getName(), ConfigConsts::configRootNodeName);

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

// open file including one file
template<>
template<>
void testObj::test<7>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/include_one.xml");
  const XML::Node  r =cfr.getTree().getRoot();
  const XML::Node &sn=r.getChild("some_node");
  ensure_equals("invalid node's value", sn.getChild("key").getValuesString(), "question");
  ensure_equals("invalid includes node 1 value",     sn.getChild("nodeX").getValuesString(), "");
  ensure_equals("invalid includes node 1 attribute", sn.getChild("nodeX").getAttributesList().getAttribute("at").getValue(), "here");
  ensure_equals("invalid includes node 2 value",     sn.getChild("nodeY").getValuesString(), "d00m");
  ensure_equals("invalid includes node 3.1 value",   sn.getChild("nodeZ").getChild("a").getValuesString(), "");
  ensure_equals("invalid includes node 3.2 value",   sn.getChild("nodeZ").getChild("b").getValuesString(), "");
}

// open file including two files
template<>
template<>
void testObj::test<8>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/include_multiple.xml");
  const XML::Node  r =cfr.getTree().getRoot();
  const XML::Node &sn=r.getChild("some_node");
  ensure_equals("invalid includes node 1 value", sn.getChild("nodeY").getValuesString(), "d00m");
  ensure_equals("invalid includes node 2 value", r.getChild("nodeY").getValuesString(), "d00m");
}

// open file including file that include other file
template<>
template<>
void testObj::test<9>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/deep_include.xml");
  const XML::Node  r =cfr.getTree().getRoot();
  const XML::Node &sn=r.getChild("some_node");
  ensure_equals("invalid includes node 1 value", sn.getChild("first").getChild("nodeY").getValuesString(), "d00m");
}

// tries including file that does not exist
template<>
template<>
void testObj::test<10>(void)
{
  const char *msg="including files creating loop didn't failed";
  ensureThrow<ConfigFileReader::ExceptionInclusionLoop>(msg, "testdata/cfg_file_expand/loop_includes.xml");
}

// open file including file from other directory
template<>
template<>
void testObj::test<11>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/include_from_other_dir.xml");
  const XML::Node  r =cfr.getTree().getRoot();
  const XML::Node &sn=r.getChild("some_node");
  ensure_equals("invalid includes node 1 value", sn.getChild("abc").getValuesString(), "hello evil");
}

// open file including file from other directory
template<>
template<>
void testObj::test<12>(void)
{
  const ConfigFileReader cfr("testdata/cfg_file_expand/include_dir_jumps.xml");
  const XML::Node  r =cfr.getTree().getRoot();
  const XML::Node &sn=r.getChild("some_node");
  ensure_equals("invalid includes node 1 value", sn.getChild("nodeZ").getChild("abc").getValuesString(), "hello evil");
}

// try including file that does not have "include" root node
template<>
template<>
void testObj::test<13>(void)
{
  const char *msg="including file with invalid root node name didn't failed";
  ensureThrow<ConfigFileReader::ExceptionInvalidInclude>(msg, "testdata/cfg_file_expand/include_invalid_root.xml");
}


namespace
{
struct Unlinker
{
  explicit Unlinker(const char *path):
    path_(path)
  {
    ensure("NULL path to unlink", path_!=NULL);
  }
  ~Unlinker(void)
  {
    unlink(path_);
  }

private:
  const char *path_;
}; // struct Unlinker
} // unnamed namespace

// try including file with a full path
template<>
template<>
void testObj::test<14>(void)
{
  using namespace Base::Filesystem;
  // settings
  const char *outPath="full_include.xml";
  const char *include="testdata/cfg_file_expand/basic_include.xml";
  const Path  incFull=boost::filesystem::system_complete(include);
  ensure("path name is not absolute", isAbsolute(incFull) );

  // write file with full path to include
  ofstream of;
  of.open(outPath);
  ensure("canno open temporary file", of.is_open() );
  Unlinker cleanup(outPath);
  of<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
  of<<"<"<<ConfigConsts::configRootNodeName<<">"<<endl;
  of<<"  <include>"<<incFull.string()<<"</include>"<<endl;
  of<<"</"<<ConfigConsts::configRootNodeName<<">"<<endl;
  of.close();

  // now try reading this file
  const ConfigFileReader cfr(outPath);
  const XML::Node        r=cfr.getTree().getRoot();
  ensure_equals("invalid node value after inclusion with full path", r.getChild("nodeY").getValuesString(), "d00m");
}

} // namespace tut
