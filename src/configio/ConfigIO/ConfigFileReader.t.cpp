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
  // TODO
}

// open non-xml file
template<>
template<>
void testObj::test<3>(void)
{
  // TODO
}

// check for exception when file cannot be read
template<>
template<>
void testObj::test<4>(void)
{
  typedef Exception Excp;
  const char *msg="opening file without permissions didn't caused an error";
  ensureThrow<Excp>(msg, "/etc/shadow");
}

// check for exception when file does not exist
template<>
template<>
void testObj::test<5>(void)
{
  typedef Exception Excp;
  const char *msg="opening non-exisitng file didn't caused an error";
  ensureThrow<Excp>(msg, "/file/that/does/not/exist.txt");
}

// check for exception when file does not exist, but can be created
template<>
template<>
void testObj::test<6>(void)
{
  typedef Exception Excp;
  const char *msg="opening non-exisitng file didn't caused an error";
  ensureThrow<Excp>(msg, "_file_that_does_not_exist_but_can_be_created.txt");
}

} // namespace tut
