/*
 * Data.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <algorithm>
#include <cassert>

#include "Filter/UsersMonitor/Data.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void)
  {
  }

  ConstAlertPtrNN mkAlert(const char *src1=NULL, const char *src2=NULL,
                          const char *dst1=NULL, const char *dst2=NULL,
                          const bool  force=false) const
  {
    Alert::Hosts src;
    src.push_back( mkHost(src1, src2, force) );
    Alert::Hosts tgt;
    tgt.push_back( mkHost(dst1, dst2, force) );

    return ConstAlertPtrNN( new Alert("n", Alert::Analyzers( makeNewAnalyzer() ),
                                      NULL, Timestamp(), Severity(SeverityLevel::INFO), 0.42, "d", src, tgt) );
  }

  HostPtrNN mkHost(const char *user1, const char *user2, bool force) const
  {
    Host::Processes p;
    if(user1!=NULL || force)
      p.push_back( mkProc(user1) );
    if(user2!=NULL || force)
      p.push_back( mkProc(user2) );
    return HostPtrNN( new Host(Host::IPv4::from_string("1.2.3.4"), NULL, "linux",
                               ConstReferenceURLPtr(), Host::Services(), p, NULL) );
  }

  ConstProcessPtrNN mkProc(const char *user) const
  {
    return ProcessPtrNN( new Process("/a/b", "b", NULL, NULL, NULL, user, NULL, ConstReferenceURLPtr()) );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/UsersMonitor/Data");
} // unnamed namespace


namespace tut
{

// test if creating from alert with processes w/o users is NULL.
template<>
template<>
void testObj::test<1>(void)
{
  auto_ptr<Data> d=Data::createFrom( mkAlert(NULL, NULL, NULL, NULL, true) );
  ensure("processes w/o users generated non NULL output", d.get()==NULL );
}

// test if creating from alert w/o processes is NULL.
template<>
template<>
void testObj::test<2>(void)
{
  auto_ptr<Data> d=Data::createFrom( mkAlert(NULL, NULL, NULL, NULL, false) );
  ensure("alert w/o processes generated non NULL output", d.get()==NULL );
}

// test if collected values are fine
template<>
template<>
void testObj::test<3>(void)
{
  auto_ptr<Data> d=Data::createFrom( mkAlert("alice", "cat", "tom", "jerry") );
  ensure("got NULL", d.get()!=NULL);
  Data::Names    out=d->get();
  sort( out.begin(), out.end() );   // for sake of test's simplicity
  ensure_equals("invalid number of elements", out.size(), 4u);
  ensure_equals("invalid element 1", out[0], "alice");
  ensure_equals("invalid element 2", out[1], "cat"  );
  ensure_equals("invalid element 3", out[2], "jerry");
  ensure_equals("invalid element 4", out[3], "tom"  );
}

// test swap
template<>
template<>
void testObj::test<4>(void)
{
  auto_ptr<Data> d1=Data::createFrom( mkAlert("alice") );
  auto_ptr<Data> d2=Data::createFrom( mkAlert("cat")   );
  ensure("d1 is NULL", d1.get()!=NULL );
  ensure("d2 is NULL", d2.get()!=NULL );
  d1->swap(*d2);
  // check
  ensure_equals("invalid size 1", d1->get().size(), 1u);
  ensure_equals("invalid size 2", d2->get().size(), 1u);
  ensure_equals("invalid value 1", d1->get()[0], "cat"  );
  ensure_equals("invalid value 2", d2->get()[0], "alice");
}

// test intersection when there is none
template<>
template<>
void testObj::test<5>(void)
{
  auto_ptr<Data> d1=Data::createFrom( mkAlert("alice") );
  auto_ptr<Data> d2=Data::createFrom( mkAlert("cat")   );
  ensure("d1 is NULL", d1.get()!=NULL );
  ensure("d2 is NULL", d2.get()!=NULL );
  Data::Names::const_iterator it=d1->commonWith(*d2);
  ensure("something has been found", it==d1->get().end() );
}

// test intersection when there is common element
template<>
template<>
void testObj::test<6>(void)
{
  auto_ptr<Data> d1=Data::createFrom( mkAlert("alice", "dog") );
  auto_ptr<Data> d2=Data::createFrom( mkAlert("dog", "yeti")   );
  ensure("d1 is NULL", d1.get()!=NULL );
  ensure("d2 is NULL", d2.get()!=NULL );
  Data::Names::const_iterator it=d1->commonWith(*d2);
  ensure("nothing has been returned", it!=d1->get().end() );
  ensure_equals("invalid element returned", *it, "dog");
}

} // namespace tut
