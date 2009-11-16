/*
 * ObjectFactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Postgres/ObjectFactoryBuilder.hpp"

using namespace std;
using namespace Persistency::Postgres;

namespace
{

struct ObjectFactoryBuilderTestClass
{
  ObjectFactoryBuilder          ofb_;
  ObjectFactoryBuilder::Options opts_;
};

typedef ObjectFactoryBuilderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Postgres/ObjectFactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", ofb_.getTypeName(), "postgres");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  ObjectFactoryBuilder::FactoryPtr ptr=ofb_.build(opts_);
  ensure("NULL pointere returned", ptr!=NULL);
}

// TODO: test different options

} // namespace tut
