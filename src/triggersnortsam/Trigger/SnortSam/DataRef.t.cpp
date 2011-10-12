/*
 * DataRef.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/DataRef.hpp"

using namespace Trigger::SnortSam;

namespace
{

struct TestClass
{
  uint8_t buf_[3];
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/DataRef");
} // unnamed namespace


namespace tut
{

// test NULL pointer
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    DataRef dr(NULL, 1);
    fail("c-tor didn't throw on error");
  }
  catch(const Exception&)
  {
    // this is expected
  }
}

// test length parameter
template<>
template<>
void testObj::test<2>(void)
{
  const DataRef dr(buf_, 2);
  ensure_equals("invalid size", dr.size(), 2);
}

// test pointer
template<>
template<>
void testObj::test<3>(void)
{
  const DataRef dr(buf_, 2);
  ensure("invalid data pointer", dr.data()==buf_);
}

// test [] access
template<>
template<>
void testObj::test<4>(void)
{
  const DataRef dr(buf_, sizeof(buf_));
  buf_[1]=42;
  ensure_equals("invalid elements' value", dr[1], 42u);
}

// test copyability
template<>
template<>
void testObj::test<5>(void)
{
  const DataRef dr1(buf_, sizeof(buf_));
  const DataRef dr2(dr1);
  ensure_equals("invalid size", dr2.size(), dr1.size());
  ensure("invalid data poitner", dr1.data()==dr2.data());
}

} // namespace tut
