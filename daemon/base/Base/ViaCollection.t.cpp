/*
 * ViaCollection.t.cpp
 *
 */
#include <tut.h>
#include <list>
#include <boost/static_assert.hpp>

#include "Base/ViaCollection.hpp"

using namespace std;
using namespace Base;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ViaCollection");
} // unnamed namespace


namespace tut
{

// test empty lists
template<>
template<>
void testObj::test<1>(void)
{
  const list<int> l;
  ensure("empty lists appear as different", ViaCollection::equal(l, l) );
}

// test lists of different sizes
template<>
template<>
void testObj::test<2>(void)
{
  const list<int> l1;
  const list<int> l2(142);
  ensure("lists of different sizes appeared as the same",
         !ViaCollection::equal(l1, l2) );
}

// test vector of identical values
template<>
template<>
void testObj::test<3>(void)
{
  list<int> l1;
  for(int i=0; i<4; ++i)
    l1.push_back(12-i);
  list<int> l2=l1;
  ensure("lists of the same values differ", ViaCollection::equal(l1, l2) );
}

// test lists of pointers
template<>
template<>
void testObj::test<4>(void)
{
  list<const long*> l;
  const long values[]={5, 10, 15};
  for(unsigned int i=0; i<sizeof(values)/sizeof(values[0]); ++i)
    l.push_back( &values[i] );
  ensure("lists of pointers differ", ViaCollection::equal(l, l) );
}

// test lists of different pointers, with the same values
template<>
template<>
void testObj::test<5>(void)
{
  list<const long*> l1;
  list<const long*> l2;

  const long values1[]={5, 10, 15};
  const long values2[]={5, 10, 15};
  BOOST_STATIC_ASSERT( sizeof(values1)==sizeof(values2) );

  for(unsigned int i=0; i<sizeof(values1)/sizeof(values1[0]); ++i)
  {
    l1.push_back( &values1[i] );
    l2.push_back( &values2[i] );
  }
  // check
  ensure("lists of different pointers to same values differ",
         ViaCollection::equal(l1, l2) );
}

// test lists of different pointers to different values
template<>
template<>
void testObj::test<6>(void)
{
  list<const long*> l1;
  list<const long*> l2;

  const long values1[]={5, 10, 15};
  const long values2[]={5, 15, 10};
  BOOST_STATIC_ASSERT( sizeof(values1)==sizeof(values2) );

  for(unsigned int i=0; i<sizeof(values1)/sizeof(values1[0]); ++i)
  {
    l1.push_back( &values1[i] );
    l2.push_back( &values2[i] );
  }
  // check
  ensure("lists of pointers to different values reported to be equal",
         !ViaCollection::equal(l1, l2) );
}

// test lists of boost::shared_ptr<> to the same values
template<>
template<>
void testObj::test<7>(void)
{
  typedef boost::shared_ptr<long> LongPtr;
  list<LongPtr> l1;
  list<LongPtr> l2;

  for(int i=0; i<3; ++i)
  {
    l1.push_back( LongPtr( new long(42+i) ) );
    l2.push_back( LongPtr( new long(42+i) ) );
  }
  // check
  ensure("lists of different smart pointers to same values differ",
         ViaCollection::equal(l1, l2) );
}

// test lists of boost::shared_ptr<> to different values
template<>
template<>
void testObj::test<8>(void)
{
  typedef boost::shared_ptr<long> LongPtr;
  list<LongPtr> l1;
  list<LongPtr> l2;

  for(int i=0; i<3; ++i)
  {
    l1.push_back( LongPtr( new long(42+i) ) );
    l2.push_back( LongPtr( new long(42-i) ) );
  }
  // check
  ensure("lists of different smart pointers to different values reported equal",
         !ViaCollection::equal(l1, l2) );
}

} // namespace tut
