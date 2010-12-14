/*
 * ReverseIDCache.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/TestCache.t.hpp"
#include "Persistency/IO/Postgres/ReverseIDCache.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;

namespace
{

// strict weak ordering for std::sort()
struct SWO
{
  bool operator()(const DataBaseID &left,
                  const DataBaseID &right)
  {
    return left<right;
  }
}; // struct SWO

struct TestClass: public TestCache< ReverseIDCache<double> , double>
{
  TestClass(void)
  {
    data_.push_back( 42 );
    data_.push_back( 27 );
    data_.push_back( 69 );

    std::sort(data_.begin(), data_.end(), SWO() );
    assert( data_[0]<data_[1] );
    assert( data_[1]<data_[2] );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ReverseIDCache");
} // unnamned namespace

namespace tut
{

// test default c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Cache tc;
}
// test getting from empty set
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    tc_.get( data_[0] );
    fail("get() didn't throw for nonexisitng ID");
  }
  catch(const ExceptionNoSuchEntry&)
  {
    // this is expected
  }
}

// test getting an exisitng entry
template<>
template<>
void testObj::test<3>(void)
{
  fill(2.8);
  ensure_equals("invalid value", tc_.get( data_[1] ), 2.8 + 1.0 );
}

// test adding an entry
template<>
template<>
void testObj::test<4>(void)
{
  ensure("pre-failed", !hasElement(data_[2]) );
  tc_.add(data_[2], 1.0);
  ensure("element not added", hasElement(data_[2]) );
  ensure_equals("invalid ID", tc_.get(data_[2]), 1.0);
}

// test const version of get() method
template<>
template<>
void testObj::test<5>(void)
{
  fill(2.8);
  const Cache &cRef=tc_;
  ensure_equals("invalid data", cRef.get(data_[0]), 2.8);
}

// test const version of has() method
template<>
template<>
void testObj::test<6>(void)
{
  const Cache &cRef=tc_;
  ensure("invalid data", cRef.has(data_[0]) == false);
}
} // namespace tut
