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

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", rb_.getTypeName(), "prelude");
}

// test creating some reader
template<>
template<>
void testObj::test<2>(void)
{
  opts_["preludeProfile"]="profile";
  ReaderBuilder::FactoryPtr ptr=rb_.build(opts_);
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// TODO: test different options

} // namespace tut
