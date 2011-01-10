/*
 * Singleton.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

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
bool isNullSingletonDeallocated;
struct SomeTestData
{
  SomeTestData(void)
  {
    isNullSingletonDeallocated=false;
  }
  ~SomeTestData(void)
  {
    isNullSingletonDeallocated=true;
  }
}; // struct SomeTestData
typedef Singleton<SomeTestData> TestNullSingleton;

struct TestExitNull: public AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    // NOTE: test has been limited to external value check
    //       since Singleton checks if pointer isn't NULL
    //      by default.
    // singleton should be free and NULL'ed by now
    assert( isNullSingletonDeallocated==true &&
            "something went wrong - singleton not deallocated" );
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
  // first register checker
  AtExit::TDeallocPtr dealloc(new TestExitNull);
  AtExit::registerDeallocator(dealloc);
  // now register singleton (implicit) and test if pointer is fine
  ensure("object inside singleton not created",
         TestNullSingleton::get()!=NULL);
  assert( isNullSingletonDeallocated==false &&
          "allocation failed - cannot proceed");
}

namespace
{
int allocatedImplementationsCount=0;

template<int N>
struct CountAllocations
{
  CountAllocations(void)
  {
    ++allocatedImplementationsCount;
    boost::thread::yield();             // the most 'inaproperiate' moment
                                        // to interrupt singleton and test
  }
}; // struct CountAllocations

struct CollableCountAllocations
{
  void operator()(void)
  {
    boost::thread::yield();
    Singleton< CountAllocations<0> >::get();
    Singleton< CountAllocations<1> >::get();
    Singleton< CountAllocations<2> >::get();
    Singleton< CountAllocations<3> >::get();
    Singleton< CountAllocations<4> >::get();
    Singleton< CountAllocations<5> >::get();
    Singleton< CountAllocations<6> >::get();
    Singleton< CountAllocations<7> >::get();
    Singleton< CountAllocations<8> >::get();
    Singleton< CountAllocations<9> >::get();
    Singleton< CountAllocations<10> >::get();
    Singleton< CountAllocations<11> >::get();
    Singleton< CountAllocations<12> >::get();
    Singleton< CountAllocations<13> >::get();
    Singleton< CountAllocations<14> >::get();
    Singleton< CountAllocations<15> >::get();
    Singleton< CountAllocations<16> >::get();
    Singleton< CountAllocations<17> >::get();
    Singleton< CountAllocations<18> >::get();
    Singleton< CountAllocations<19> >::get();
    Singleton< CountAllocations<20> >::get();
    Singleton< CountAllocations<21> >::get();
    Singleton< CountAllocations<22> >::get();
    Singleton< CountAllocations<23> >::get();
    Singleton< CountAllocations<24> >::get();
    Singleton< CountAllocations<25> >::get();
    Singleton< CountAllocations<26> >::get();
    Singleton< CountAllocations<27> >::get();
    Singleton< CountAllocations<28> >::get();
    Singleton< CountAllocations<29> >::get();
  }
}; // struct CollableCountAllocations
} // unnamed namespace

// test if there is no race between multiple allocations
template<>
template<>
void testObj::test<5>(void)
{
  CollableCountAllocations cca;
  boost::thread th1(cca);
  boost::thread th2(cca);
  boost::thread th3(cca);
  boost::thread th4(cca);
  boost::thread::yield();

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  ensure_equals("invalid allocations - init() is not thread-safe",
                allocatedImplementationsCount, 30);
}


namespace
{
bool isSTDDeallocated;
struct STD
{
  STD(void)
  {
    isSTDDeallocated=false;
  }
  ~STD(void)
  {
    isSTDDeallocated=true;
  }
}; // struct STD
} // unnamed namespace

// test SingletonData c-tor/d-tor
template<>
template<>
void testObj::test<6>(void)
{
  {
    detail::SingletonData<STD> tmp(new STD);
    ensure("allocation failed", !isSTDDeallocated);
  }
  ensure("deallocation failed",  isSTDDeallocated);
}

// test SingletonData::get()
template<>
template<>
void testObj::test<7>(void)
{
  STD                        *rawPtr=new STD;
  detail::SingletonData<STD>  obj(rawPtr);
  ensure("allocation failed", !isSTDDeallocated);
  ensure("invalid pointer", rawPtr==obj.get().get() );
}

// test deallocation
template<>
template<>
void testObj::test<8>(void)
{
  detail::SingletonData<STD> obj(new STD);
  ensure("allocation failed", !isSTDDeallocated);
  ensure("NULL pointer allocated", obj.get().get()!=NULL );
  ensure("expired() is true too soon", obj.expired()==false);

  obj.prepareToDeallocate();
  ensure("NULL pointer allocated", obj.get().get()==NULL );
  ensure("expired() is false after deallocation", obj.expired()==true);
}

// test deallocation when copies still exist
template<>
template<>
void testObj::test<9>(void)
{
  detail::SingletonData<STD> obj(new STD);
  ensure("allocation failed", !isSTDDeallocated);
  boost::shared_ptr<STD>     tmp=obj.get();
  ensure("NULL pointer allocated", obj.get().get()!=NULL );
  ensure("expired() is true too soon", obj.expired()==false);

  obj.prepareToDeallocate();
  ensure("deallocation of exisitng object", obj.get().get()!=NULL );
  ensure("expired() is true when instances exist", obj.expired()==false);

  tmp.reset();
  ensure("removing last instance didn't changed anything", obj.get().get()==NULL );
  ensure("expired() is false after deallocation", obj.expired()==true);
}


namespace
{
struct ExampleDataStruct
{
  void foo(void)
  {
  }
}; // struct ExampleDataStruct

typedef Singleton<ExampleDataStruct> ExampleSingleton;
} // unnamed namespace

// test getting pointer wrapper
template<>
template<>
void testObj::test<10>(void)
{
  ExampleSingleton::PointerWrapper pw=ExampleSingleton::get();
  pw->foo();
}

// test getting raw pointer
template<>
template<>
void testObj::test<11>(void)
{
  ExampleDataStruct *ptr=ExampleSingleton::get();
  ptr->foo();
}


namespace
{
bool dataPostExists;
struct DataPost
{
  DataPost(void)
  {
    dataPostExists=true;
  }
  ~DataPost(void)
  {
    dataPostExists=false;
  }
}; // struct DataPost

struct DataPre
{
  ~DataPre(void)
  {
    assert(dataPostExists==false && "d-tor of DataPre not called");
    DataPost *ptr=Singleton<DataPost>::get();
    assert(dataPostExists==true && "phoenix didn't araised from ashes");
    assert(ptr!=NULL && "NULL pointer recieved, though phoenix raised from ashes");
  }
}; // struct DataPtr
} // unnamed namespace

// test getting pointer wrapper
template<>
template<>
void testObj::test<12>(void)
{
  Singleton<DataPre>::get();    // first init
  Singleton<DataPost>::get();   // second init
  ensure("singleton not raised - it will fail...", dataPostExists==true);
  // when d-tor will be called in atexit() it will be firstly called for
  // Singleton<DataPost> instance. Singleton<DataPte> should use phoenix
  // singleton's availability to overcome this issue
}

} // namespace tut
