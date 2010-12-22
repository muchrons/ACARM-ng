/*
 * Loader.t.cpp
 *
 */
#include <tut.h>
#include <boost/filesystem.hpp>

#include "Plugins/Loader.hpp"
#include "Commons/Filesystem/isFileSane.hpp"

using namespace std;
using namespace Plugins;
namespace fs=boost::filesystem;

namespace
{

struct TestClass
{
  template<typename T>
  void testThrow(const fs::path &p)
  {
    try
    {
      Loader load(p);
      tut::fail("c-tor didn't failed");
    }
    catch(const T &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Plugins/Loader");
} // unnamed namespace


namespace tut
{

// try reading from empty dir
template<>
template<>
void testObj::test<1>(void)
{
  const Loader load("testdata/empty_dir");
  ensure_equals("invalid number of plugins", load.loadedCount(), 0u);
}

// test reading plugin
template<>
template<>
void testObj::test<2>(void)
{
  const fs::path file("_plugin_has_been_initialized_");
  fs::remove(file);
  // test loading plugin
  const Loader load("testdata/valid_plugin_dir");
  ensure_equals("invalid number of plugins", load.loadedCount(), 1u);
  // check if plugin has been initialized
  ensure("plugin not initialized", Commons::Filesystem::isFileSane(file) );
  fs::remove(file);
}

// test exceptionon invalid plugin
template<>
template<>
void testObj::test<3>(void)
{
  testThrow<ExceptionInvalidPlugin>("testdata/invalid_plugin_dir");
}

// test direcotry with some content, but no plugins
template<>
template<>
void testObj::test<4>(void)
{
  const Loader load("testdata/no_plugins_dir");
  ensure_equals("invalid number of plugins", load.loadedCount(), 0u);
}

// test exception on non-existing directory
template<>
template<>
void testObj::test<5>(void)
{
  testThrow<ExceptionInvalidDirectory>("/some/non/existing/directory");
}

// test exception on directory that is symlink
template<>
template<>
void testObj::test<6>(void)
{
  testThrow<ExceptionInvalidDirectory>("testdata/symlink_to_dir");
}

} // namespace tut
