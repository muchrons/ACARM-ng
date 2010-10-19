/*
 * ReaderBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Input/File/ReaderBuilder.hpp"

using namespace std;
using namespace Input::File;

namespace
{

struct ReaderBuilderTestClass
{
  ReaderBuilder          rb_;
  ReaderBuilder::Options opts_;
};

typedef ReaderBuilderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/File/ReaderBuilder");
} // unnamed namespace


namespace tut
{

// test getting type
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", rb_.getTypeName(), "file");
}

// test creating some reader
template<>
template<>
void testObj::test<2>(void)
{
  opts_["name"]="somename";
  opts_["fifo"]="some_fifo";
  ReaderBuilder::FactoryPtr ptr=rb_.build(opts_);
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test exception on missing path
template<>
template<>
void testObj::test<3>(void)
{
  //opts_["fifo"]="some_fifo";
  opts_["name"]="whatever";
  try
  {
    rb_.build(opts_);
    fail("builder didn't throw on missing fifo path");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

// test exception on missing name
template<>
template<>
void testObj::test<4>(void)
{
  opts_["fifo"]="some_fifo";
  //opts_["name"]="whatever";
  try
  {
    rb_.build(opts_);
    fail("builder didn't throw on missing name");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

} // namespace tut
