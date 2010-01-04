/*
 * TimeoutQueueIterator.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/TimeoutQueue.hpp"

using namespace std;
using namespace tut;
using namespace Base;

namespace
{
struct TestClass
{
  typedef double                                 Type;
  typedef std::vector< std::pair<Type, time_t> > DequeImpl;
  typedef TimeoutQueue<Type>                     Queue;


  typedef Queue::iterator       Iter;
  typedef Queue::const_iterator ConstIter;


  TestClass(void)
  {
    qImpl2_.push_back( make_pair(42.0, 123) );
    qImpl2_.push_back( make_pair(4.2,  234) );
  }

  // since (nearly) all tests are the same for const and non-const iterators
  // templates have been provided to implement these tests in common way.

  template<typename IT>
  void testEmptyCollectionInit(void)
  {
    IT begin( qImpl1_.begin() );
    IT end( qImpl1_.end() );
    ensure("invalid iterators", begin==end);
  }

  template<typename IT>
  void testPreIncrementingIterator(void)
  {
    IT begin( qImpl2_.begin() );
    IT end( qImpl2_.end() );
    ensure("invalid iterators", begin!=end);
    ++begin;
    ensure("incrementation failed", begin!=end);
    ++begin;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT, typename Tmp>
  void testDereference(void)
  {
    IT begin( qImpl2_.begin() );
    // first
    {
      Tmp tmp=*begin;
      ensure_equals("invalid value 1", tmp, 42);
      ++begin;
    }
    // second
    {
      Tmp tmp=*begin;
      ensure_equals("invalid value 2", tmp, 4.2);
    }
  }

  template<typename IT>
  void testPostIncrementingIterator(void)
  {
    IT begin( qImpl2_.begin() );
    IT end( qImpl2_.end() );
    ensure("invalid iterators", begin!=end);
    begin++;
    ensure("incrementation failed", begin!=end);
    begin++;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT>
  void testPreDecrementingIterator(void)
  {
    IT begin( qImpl2_.begin() );
    IT end( qImpl2_.end() );
    ensure("invalid iterators", begin!=end);
    --end;
    ensure("incrementation failed", begin!=end);
    --end;
    ensure("problem whne re-incrementing", begin==end);
  }

  template<typename IT>
  void testPostDecrementingIterator(void)
  {
    IT begin( qImpl2_.begin() );
    IT end( qImpl2_.end() );
    ensure("invalid iterators", begin!=end);
    end--;
    ensure("incrementation failed", begin!=end);
    end--;
    ensure("problem whne re-incrementing", begin==end);
  }

  DequeImpl qImpl1_;
  DequeImpl qImpl2_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/TimeoutQueueIterator");
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

// try converting iterator to const-iterator via c-tor
template<>
template<>
void testObj::test<13>(void)
{
  Iter      it( qImpl2_.begin() );
  ConstIter cit(it);
  ensure_equals("iterators point ot different places", *it, *cit);
}

// try converting iterator to const-iterator via assignment
template<>
template<>
void testObj::test<14>(void)
{
  Iter      it( qImpl2_.end() );
  ConstIter cit( qImpl2_.begin() );
  ensure("pre-condition failed", *it!=*cit);
  cit=it;
  ensure_equals("iterators point ot different places", *it, *cit);
}

// test copying iterators of the same type
template<>
template<>
void testObj::test<15>(void)
{
  Iter it1( qImpl1_.begin() );
  Iter it2=it1;
}

// test copying iterators of the same type - const
template<>
template<>
void testObj::test<16>(void)
{
  ConstIter it1( qImpl1_.begin() );
  ConstIter it2=it1;
}

} // namespace tut
