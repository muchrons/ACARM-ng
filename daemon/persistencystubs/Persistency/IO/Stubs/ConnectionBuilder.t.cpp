/*
 * ObjectFactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Stubs/ConnectionBuilder.hpp"

using namespace std;
using namespace Persistency::IO::Stubs;

namespace
{

struct TestClass
{
  ConnectionBuilder          ofb_;
  ConnectionBuilder::Options opts_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Stubs/ConnectionBuilder");
} // unnamed namespace


namespace tut
{

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", ofb_.getTypeName(), "stubs");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  ConnectionBuilder::FactoryPtr ptr=ofb_.build(opts_);
  ensure("NULL pointere returned", ptr!=NULL);
}

// TODO: test different options

} // namespace tut
