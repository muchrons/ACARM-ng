/*
 * ReverseIDCache.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/ReverseIDCache.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;

namespace
{

// TODO: c&p code from StorageDataCache.t.cpp - make common code common.

// strict weak ordering for std::sort()
struct SWO
{
  bool operator()(const DataBaseID &left,
                  const DataBaseID &right)
  {
    return left<right;
  }
}; // struct SWO

struct TestClass
{

  typedef ReverseIDCache<double> TestCache;

  TestClass(void)
  {
    data_.push_back( 42 );
    data_.push_back( 27 );
    data_.push_back( 69 );

    std::sort(data_.begin(), data_.end(), SWO() );
    assert( data_[0]<data_[1] );
    assert( data_[1]<data_[2] );
  }


  void fill(void)
  {
    double val=2.8;
    for(size_t i=0; i<data_.size(); ++i)
    {
      tc_.add(data_[i], val);
      ++val;
    }
  }

  bool hasElement(DataBaseID id)
  {
    try
    {
      tc_.get(id);
      return true;
    }
    catch(const ExceptionNoSuchEntry&)
    {
      return false;
    }
  }

  std::vector<DataBaseID> data_;
  TestCache           tc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ReverseIDCache");
} //unnamned namespace

namespace tut
{

// test default c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  TestCache tc;
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
  fill();
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
  fill();
  const TestCache &cRef=tc_;
  ensure_equals("invalid data", cRef.get(data_[0]), 2.8);
}

// test const version of has() method
template<>
template<>
void testObj::test<6>(void)
{
  const TestCache &cRef=tc_;
  ensure("invalid data", cRef.has(data_[0]) == false);
}
} // namespace tut
