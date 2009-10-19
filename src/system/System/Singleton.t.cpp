/*
 * Singleton.t.cpp
 *
 */
#include <tut.h>

#include "System/Singleton.hpp"

namespace
{
struct MySingleType
{
  void inc(void)
  {
    ++_cnt;
  }

  int get(void) const
  {
    return _cnt;
  }

private:
  friend class System::Singleton<MySingleType>;

  MySingleType(void):
    _cnt(0)
  {
  }

  int _cnt;
}; // struct MySingleType
} // unnamed namespace


namespace System
{

struct SingletonTestClass
{
};

} // namespace System


namespace tut
{
typedef System::SingletonTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Singleton");
}

typedef System::Singleton<MySingleType> TestSingleton;

using namespace System;


namespace tut
{

// test get/increment
template<>
template<>
void testObj::test<1>(void)
{
  ensure( TestSingleton::get()->get()==0 );

  TestSingleton::get()->inc();
  ensure( TestSingleton::get()->get()==1 );

  TestSingleton::get()->inc();
  ensure( TestSingleton::get()->get()==2 );
}

// test if it is truely global
template<>
template<>
void testObj::test<2>(void)
{
  TestSingleton::get()->inc();
  ensure( TestSingleton::get()->get()==3 );
}

namespace
{

// table of destroyed OrderSingleton marks. each destroyed
// singleton wil mark its absence with flag here
bool tab[6]={false, false, false,
             false, false, false};

template<int N>
struct OrderSingleton
{
  ~OrderSingleton(void)
  {
    for(int i=0; i<N; ++i)      // ceck desctruction order 1
      assert(tab[i]==true);

    tab[N]=true;                // mark self as destroyed

    const int s=sizeof(tab)/sizeof(tab[0]);
    for(int i=N+1; i<s; ++i)    // ceck desctruction order 2
      assert(tab[i]==false);
  }
}; // struct MySingleType
} // unnamed namespace

// test if creation order reflects destruction order.
// this is smoke test - if it won't crash at exit, it
// means it works fine.
template<>
template<>
void testObj::test<3>(void)
{
  Singleton< OrderSingleton<5> >::get();    // create
  Singleton< OrderSingleton<4> >::get();    // create
  Singleton< OrderSingleton<3> >::get();    // create
  Singleton< OrderSingleton<2> >::get();    // create
  Singleton< OrderSingleton<1> >::get();    // create
  Singleton< OrderSingleton<0> >::get();    // create
  // dealocation should that place in reverse order
}

namespace
{
struct SomeTestData
{
  int x;
}; // struct SomeTestData
typedef Singleton<SomeTestData> TestNullSingleton;

struct TestExitNull: public AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    // NOTE: test has been commented since Singleton checks
    //       if pointer isn't NULL by default.
    // pointer should be free and NULL'ed by now
    //assert( TestNullSingleton::get()==NULL );
  }
}; // struct TestExitNull
} // unnamed namespace

// test if get after deallocation returns NULL. this
// is a smoke test - if it doesn't crash at exit, it
// means it works fine.
template<>
template<>
void testObj::test<4>(void)
{
  // at first register checker
  AtExit::TDeallocPtr dealloc(new TestExitNull);
  AtExit::registerDeallocator(dealloc);
  // now register singleton (implicit) and test if pointer is fine
  ensure("object inside singleton not created",
         TestNullSingleton::get()!=NULL);
}

} // namespace tut

