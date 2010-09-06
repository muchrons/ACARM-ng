/*
 * ViaUnorderedCollection.t.cpp
 *
 */
#include <tut.h>
#include <list>
#include <vector>
#include <boost/static_assert.hpp>

#include "Commons/ViaUnorderedCollection.hpp"

using namespace std;
using namespace Commons;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/ViaUnorderedCollection");
} // unnamed namespace


namespace tut
{

// test empty lists
template<>
template<>
void testObj::test<1>(void)
{
  const list<int> l;
  ensure("empty lists appear as different", ViaUnorderedCollection::equal(l, l) );
}

// test lists of different sizes
template<>
template<>
void testObj::test<2>(void)
{
  const list<int> l1;
  const list<int> l2(142);
  ensure("lists of different sizes appeared as the same",
         !ViaUnorderedCollection::equal(l1, l2) );
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
  ensure("lists of the same values differ", ViaUnorderedCollection::equal(l1, l2) );
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
  ensure("lists of pointers differ", ViaUnorderedCollection::equal(l, l) );
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
         ViaUnorderedCollection::equal(l1, l2) );
}

// test lists of different pointers to different values
template<>
template<>
void testObj::test<6>(void)
{
  list<const long*> l1;
  list<const long*> l2;

  const long values1[]={5, 10, 16};
  const long values2[]={5, 15, 10};
  BOOST_STATIC_ASSERT( sizeof(values1)==sizeof(values2) );

  for(unsigned int i=0; i<sizeof(values1)/sizeof(values1[0]); ++i)
  {
    l1.push_back( &values1[i] );
    l2.push_back( &values2[i] );
  }
  // check
  ensure("lists of pointers to different values reported to be equal",
         !ViaUnorderedCollection::equal(l1, l2) );
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
  ensure("lists of different smart pointers to the same values differ",
         ViaUnorderedCollection::equal(l1, l2) );
}

// test lists of boost::shared_ptr<> of different values
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
         !ViaUnorderedCollection::equal(l1, l2) );
}

// test lists of SharedPtrNotNULL<> of the same values
template<>
template<>
void testObj::test<9>(void)
{
  typedef SharedPtrNotNULL<long> LongPtr;
  list<LongPtr> l1;
  list<LongPtr> l2;

  for(int i=0; i<3; ++i)
  {
    l1.push_back( LongPtr( new long(42+i) ) );
    l2.push_back( LongPtr( new long(42+i) ) );
  }
  // check
  ensure("lists of different smart pointers to the same values are not equal",
         ViaUnorderedCollection::equal(l1, l2) );
}

// test lists of SharedPtrNotNULL<> to different values
template<>
template<>
void testObj::test<10>(void)
{
  typedef SharedPtrNotNULL<long> LongPtr;
  list<LongPtr> l1;
  list<LongPtr> l2;

  for(int i=0; i<3; ++i)
  {
    l1.push_back( LongPtr( new long(42+i) ) );
    l2.push_back( LongPtr( new long(42-i) ) );
  }
  // check
  ensure("lists of different smart pointers to different values reported equal",
         !ViaUnorderedCollection::equal(l1, l2) );
}

// test different collection types for inequality
template<>
template<>
void testObj::test<11>(void)
{
  list<long>   c1;
  vector<long> c2;

  for(int i=0; i<3; ++i)
  {
    c1.push_back(42+i);
    c2.push_back(42-i);
  }
  // check
  ensure("different collections of different values reported equal",
         !ViaUnorderedCollection::equal(c1, c2) );
}

// test different collection types for equality
template<>
template<>
void testObj::test<12>(void)
{
  vector<long> c1;
  list<long>   c2;

  for(int i=0; i<3; ++i)
  {
    c1.push_back(42+i);
    c2.push_back(42+i);
  }
  // check
  ensure("different collections of the same values reported equal",
         ViaUnorderedCollection::equal(c1, c2) );
}

// test collection of elements in different order
template<>
template<>
void testObj::test<13>(void)
{
  vector<long> c1;
  list<long>   c2;

  // c1
  c1.push_back(1);
  c1.push_back(2);
  c1.push_back(3);
  // c2
  c2.push_back(3);
  c2.push_back(2);
  c2.push_back(1);

  // check
  ensure("colection of the same values in different order does not match",
         ViaUnorderedCollection::equal(c1, c2) );
}

// test collections with values that repeats
template<>
template<>
void testObj::test<14>(void)
{
  vector<long> c1;
  list<long>   c2;

  // c1
  c1.push_back(1);
  c1.push_back(2);
  c1.push_back(2);
  // c2
  c2.push_back(2);
  c2.push_back(2);
  c2.push_back(1);

  // check
  ensure("collection with repetitions does not match",
         ViaUnorderedCollection::equal(c1, c2) );
}

// test for negative match, when elements repeat
template<>
template<>
void testObj::test<15>(void)
{
  vector<long> c1;
  list<long>   c2;

  // c1
  c1.push_back(1);
  c1.push_back(2);
  c1.push_back(2);
  // c2
  c2.push_back(3);
  c2.push_back(2);
  c2.push_back(1);

  // check (in both orders)
  ensure("match didn't failed when elements repeat (order 1)",
         !ViaUnorderedCollection::equal(c1, c2) );
  ensure("match didn't failed when elements repeat (order 2)",
         !ViaUnorderedCollection::equal(c2, c1) );
}

} // namespace tut
