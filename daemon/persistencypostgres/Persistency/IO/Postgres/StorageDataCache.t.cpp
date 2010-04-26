/*
 * StorageDataCache.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <algorithm>
#include <cassert>

#include "Persistency/IO/Postgres/StorageDataCache.hpp"

using namespace std;
using namespace Persistency::IO::Postgres;
namespace
{

// strict weak ordering for std::sort()
struct SWO
{
  bool operator()(const boost::shared_ptr<double> &left,
                  const boost::shared_ptr<double> &right)
  {
    return left.get()<right.get();
  }
}; // struct SWO

struct TestClass
{
  typedef StorageDataCache<double> TestStorage;

  TestClass(void)
  {
    data_.push_back( makeDouble(4.2) );
    data_.push_back( makeDouble(2.7) );
    data_.push_back( makeDouble(6.9) );

    std::sort(data_.begin(), data_.end(), SWO() );
    assert( data_[0].get()<data_[1].get() );
    assert( data_[1].get()<data_[2].get() );
  }

  boost::shared_ptr<double> makeDouble(double v) const
  {
    return boost::shared_ptr<double>( new double(v) );
  }

  void fill(void)
  {
    int id=420;
    for(size_t i=0; i<data_.size(); ++i)
    {
      ts_.add(data_[i], id);
      ++id;
    }
  }

  bool hasElement(boost::shared_ptr<double> e)
  {
    try
    {
      ts_.get(e);
      return true;
    }
    catch(const TestStorage::ExceptionNoSuchEntry&)
    {
      return false;
    }
  }

  std::vector< boost::shared_ptr<double> > data_;
  TestStorage                              ts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/StorageDataCache");
} // unnamed namespace


namespace tut
{

// test default c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  TestStorage ts;
}

// test getting from empty set
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    ts_.get( data_[0] );
    fail("get() didn't throw for nonexisitng pointer");
  }
  catch(const TestStorage::ExceptionNoSuchEntry&)
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
  ensure_equals("invalid ID", ts_.get( data_[1] ), 420+1);
}

// test adding an entry
template<>
template<>
void testObj::test<4>(void)
{
  ensure("pre-failed", !hasElement(data_[2]) );
  ts_.add(data_[2], 123);
  ensure("element not added", hasElement(data_[2]) );
  ensure_equals("invalid ID", ts_.get(data_[2]), 123);
}

// test prunning empty tree
template<>
template<>
void testObj::test<5>(void)
{
  ts_.prune();
}

// test prunning when no NULLs are present
template<>
template<>
void testObj::test<6>(void)
{
  fill();
  ts_.prune();
  // check if all elements are present and valid
  for(size_t i=0; i<data_.size(); ++i)
    ensure_equals("invalid element", ts_.get(data_[i]), 420+static_cast<int>(i) );
}

// test prunning when NULL is present
template<>
template<>
void testObj::test<7>(void)
{
  fill();
  data_.erase( data_.begin() );
  assert( data_.size()==2 );
  ts_.prune();
  // check if all elements are present and valid
  for(size_t i=0; i<data_.size(); ++i)
    ensure_equals("invalid element", ts_.get(data_[i]), 420+static_cast<int>(i)+1);
}

// test prunning when all elements are NULLs
template<>
template<>
void testObj::test<8>(void)
{
  fill();
  // remove all elements
  while( data_.size()>0 )
    data_.erase( data_.begin() );
  // test
  ts_.prune();
  ensure_equals("invalid output size", ts_.size(), 0u);
}

// test size of collection
template<>
template<>
void testObj::test<9>(void)
{
  fill();
  ensure_equals("invalid size", ts_.size(), 3u);
}

// test adding dupliacete entry
template<>
template<>
void testObj::test<10>(void)
{
  fill();
  try
  {
    ts_.add(data_[0], 123);
    fail("add() didn't throw for duplicated entry");
  }
  catch(const TestStorage::ExceptionEntryAlreadyExist&)
  {
    // this is expected
  }
}

// test const version of get function
template<>
template<>
void testObj::test<11>(void)
{
  fill();
  const TestStorage &cRef=ts_;
  ensure_equals("invalid data", cRef.get(data_[0]), 420);
}

// test const version of has function
template<>
template<>
void testObj::test<12>(void)
{
  const TestStorage &cRef=ts_;
  ensure("invalid data", cRef.has(data_[0]) == false);
}
} // namespace tut
