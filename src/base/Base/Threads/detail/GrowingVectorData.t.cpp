/*
 * GrowingVectorData.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Base/Threads/detail/GrowingVectorData.hpp"

using namespace Base::Threads::detail;

namespace
{
typedef GrowingVectorData<std::string> GVD;

struct TestClass
{
  TestClass(void)
  {
    gvd_.vec_.push_back("aaa");
    gvd_.vec_.push_back("XYZ");
  }

  GVD gvd_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/detail/GrowingVectorData");
} // unnamed namespace


namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  GVD tmp;
  ensure_equals("invalid new collection size", tmp.vec_.size(),  0u);
  ensure_equals("invalid collection size",     gvd_.vec_.size(), 2u);
}

// test copy c-tor
template<>
template<>
void testObj::test<2>(void)
{
  const GVD tmp(gvd_);
  ensure_equals("copy is invalid", tmp.vec_.size(), gvd_.vec_.size() );
}

// test assignment operator
template<>
template<>
void testObj::test<3>(void)
{
  GVD tmp;
  tmp=gvd_;
  ensure_equals("copy is invalid", tmp.vec_.size(), gvd_.vec_.size() );
}

// test assigning to self (should not fail).
template<>
template<>
void testObj::test<4>(void)
{
  const GVD &tmp=(gvd_=gvd_);
  ensure_equals("selft assignment failed", gvd_.vec_.size(), 2u);
  ensure_equals("invalid returned value",  tmp.vec_.size(),  2u);
}

} // namespace tut
