/*
 * GrowingVectorIterator.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

#include "Base/Threads/GrowingVectorIterator.hpp"

using namespace std;
using namespace tut;
using namespace Base::Threads;

namespace
{
struct TestClass
{
  typedef std::string                     Type;
  typedef detail::GrowingVectorData<Type> GD;
  typedef boost::shared_ptr<GD>           GDPtr;

  typedef GrowingVectorIterator<Type>       Iter;
  typedef GrowingVectorIterator<const Type> ConstIter;

  TestClass(void):
    data1_(new GD),
    data2_(new GD)
  {
    data2_->vec_.push_back("Alice has a cat");
    data2_->vec_.push_back("The Answer is 42");
  }

  // since (nearly) all tests are the same for const and non-const iterators
  // templates have been provided to implement these tests in common way.

  template<typename IT>
  void testEmptyCollectionInit(void)
  {
    IT begin(data1_);
    IT end(data1_, data1_->vec_.size() );
    ensure("invalid iterators", begin==end);
  }

  template<typename IT>
  void testPreIncrementingIterator(void)
  {
    IT begin(data2_);
    IT end(data2_, data2_->vec_.size() );
    ensure("invalid iterators", begin!=end);
    ++begin;
    ensure("incrementation failed", begin!=end);
    ++begin;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT, typename Tmp>
  void testDereference(void)
  {
    IT begin(data2_);
    // first
    {
      Tmp str=*begin;
      ensure_equals("invalid value 1", str, "Alice has a cat");
      ++begin;
    }
    // second
    {
      Tmp str=*begin;
      ensure_equals("invalid value 2", str, "The Answer is 42");
    }
  }

  template<typename IT>
  void testPostIncrementingIterator(void)
  {
    IT begin(data2_);
    IT end(data2_, data2_->vec_.size() );
    ensure("invalid iterators", begin!=end);
    begin++;
    ensure("incrementation failed", begin!=end);
    begin++;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT>
  void testPreDecrementingIterator(void)
  {
    IT begin(data2_);
    IT end(data2_, data2_->vec_.size() );
    ensure("invalid iterators", begin!=end);
    --end;
    ensure("incrementation failed", begin!=end);
    --end;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT>
  void testPostDecrementingIterator(void)
  {
    IT begin(data2_);
    IT end(data2_, data2_->vec_.size() );
    ensure("invalid iterators", begin!=end);
    end--;
    ensure("incrementation failed", begin!=end);
    end--;
    ensure("problem whne re-incrementing", begin==end);
  }

  GDPtr     data1_;
  GDPtr     data2_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/GrowingVectorIterator");
} // unnamed namespace


namespace tut
{

// testing initialization for empty collection
template<>
template<>
void testObj::test<1>(void)
{
  testEmptyCollectionInit<Iter>();
}

// test initialization of const iterators for empty collection
template<>
template<>
void testObj::test<2>(void)
{
  testEmptyCollectionInit<ConstIter>();
}

// test pre incrementing iterator
template<>
template<>
void testObj::test<3>(void)
{
  testPreIncrementingIterator<Iter>();
}

// test pre incrementing const iterator
template<>
template<>
void testObj::test<4>(void)
{
  testPreIncrementingIterator<ConstIter>();
}

// try dereferening
template<>
template<>
void testObj::test<5>(void)
{
  testDereference<Iter, Type>();
}

// try dereferencing const iterator
template<>
template<>
void testObj::test<6>(void)
{
  testDereference<ConstIter, const Type>();
}

// test post incrementing iterator
template<>
template<>
void testObj::test<7>(void)
{
  testPostIncrementingIterator<Iter>();
}

// test post incrementing const iterator
template<>
template<>
void testObj::test<8>(void)
{
  testPostIncrementingIterator<ConstIter>();
}

// test pre incrementing iterator
template<>
template<>
void testObj::test<9>(void)
{
  testPreDecrementingIterator<Iter>();
}

// test pre incrementing const iterator
template<>
template<>
void testObj::test<10>(void)
{
  testPreDecrementingIterator<ConstIter>();
}

// test post incrementing iterator
template<>
template<>
void testObj::test<11>(void)
{
  testPostDecrementingIterator<Iter>();
}

// test post incrementing const iterator
template<>
template<>
void testObj::test<12>(void)
{
  testPostDecrementingIterator<ConstIter>();
}

} // namespace tut
