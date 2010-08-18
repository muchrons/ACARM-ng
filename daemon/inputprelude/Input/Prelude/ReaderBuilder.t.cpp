/*
 * ReaderBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Input/Prelude/ReaderBuilder.hpp"

using namespace std;
using namespace Input::Prelude;

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

factory tf("Input/Prelude/ReaderBuilder");
} // unnamed namespace


namespace tut
{

// tets getting type
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", rb_.getTypeName(), "prelude");
}

// test creating some reader
template<>
template<>
void testObj::test<2>(void)
{
  opts_["name"]="somereadername";
  opts_["profile"]="profile";
  opts_["config"]="/etc/prelude/default/client.conf";
  ReaderBuilder::FactoryPtr ptr=rb_.build(opts_);
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// TODO: test different options

} // namespace tut
