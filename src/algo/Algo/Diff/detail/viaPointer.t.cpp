/*
 * viaPointer.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/viaPointer.hpp"
#include "Algo/Diff/detail/all.hpp"

using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;

namespace
{

struct TestType
{
}; // struct TestType

Similarity compare(const TestType &/*t1*/, const TestType &/*t2*/)
{
  return 0.424242;
} // compare()

struct TestClass
{
  template<typename T>
  void testPtr(const T *p1, const T *p2, double expected) const
  {
    const Similarity s=Algo::Diff::detail::compare(p1, p2);
    tut::ensure_equals("invalid similarity", s.get(), expected);
  }

  template<typename T>
  void testSmartPtr(const T &p1, const T &p2, double expected) const
  {
    const Similarity s=Algo::Diff::detail::compare(p1, p2);
    tut::ensure_equals("invalid similarity", s.get(), expected);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/viaPointer");
} // unnamed namespace


namespace tut
{

// check two NULL pointers
template<>
template<>
void testObj::test<1>(void)
{
  testPtr<TestType>(NULL, NULL, 1);
}

// test first NULL
template<>
template<>
void testObj::test<2>(void)
{
  const TestType t=TestType();
  testPtr<TestType>(NULL, &t, 0);
}

// test second NULL
template<>
template<>
void testObj::test<3>(void)
{
  const TestType t=TestType();
  testPtr<TestType>(&t, NULL, 0);
}

// test two non-NULLs (identical pointer)
template<>
template<>
void testObj::test<4>(void)
{
  const TestType t=TestType();
  testPtr<TestType>(&t, &t, 1);
}

// test two non-NULLs (different pointer)
template<>
template<>
void testObj::test<5>(void)
{
  const TestType t1=TestType();
  const TestType t2=TestType();
  testPtr<TestType>(&t1, &t2, 0.424242);
}

// test const char *
template<>
template<>
void testObj::test<6>(void)
{
  const char s1[]="Alice";
  const char s2[]="Alice";
  testPtr<char>(s1, s2, 1);
}

// test comaprison via boost::shared_ptr<>
template<>
template<>
void testObj::test<7>(void)
{
  typedef boost::shared_ptr<TestType> Ptr;
  const Ptr p1(new TestType);
  const Ptr p2(new TestType);
  testSmartPtr<Ptr>(p1, p2, 0.424242);
}

// test comaprison via Commons::SharedPtrNotNULL<>
template<>
template<>
void testObj::test<8>(void)
{
  typedef Commons::SharedPtrNotNULL<TestType> Ptr;
  const Ptr p1(new TestType);
  const Ptr p2(new TestType);
  testSmartPtr<Ptr>(p1, p2, 0.424242);
}

} // namespace tut
