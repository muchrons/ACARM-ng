/*
 * Severity.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Severity.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestImpl: public Severity
{
  explicit TestImpl(SeverityLevel sl, bool *dtor=NULL):
    Severity(sl),
    dtor_(dtor)
  {
    if(dtor_!=NULL)
      *dtor_=false;
  }

  virtual ~TestImpl(void)
  {
    if(dtor_!=NULL)
      *dtor_=true;
  }

  bool *dtor_;
};

struct SeverityTestClass
{
  void check(const SeverityLevel sl, const char *name) const
  {
    assert(name!=NULL);
    const TestImpl ti(sl);
    tut::ensure_equals("invalid severity level",
                       ti.getLevel().toInt(), sl.toInt() );
    tut::ensure_equals("invalid severity name",
                       ti.getName(),          string(name) );
  }
};

typedef SeverityTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Severity");
} // unnamed namespace


namespace tut
{

// check if d-tor is virtual
template<>
template<>
void testObj::test<1>(void)
{
  bool dtor;
  std::auto_ptr<Severity> ptr( new TestImpl(SeverityLevel::INFO, &dtor) );
  ensure("precondition failed", !dtor);
  ptr.reset();
  ensure("d-tor is not virtual", dtor);
}

// check level "DEBUG"
template<>
template<>
void testObj::test<2>(void)
{
  check(SeverityLevel::DEBUG, "debug");
}

// check INFO
template<>
template<>
void testObj::test<3>(void)
{
  check(SeverityLevel::INFO, "info");
}

// check NOTICE
template<>
template<>
void testObj::test<4>(void)
{
  check(SeverityLevel::NOTICE, "notice");
}

// check warning
template<>
template<>
void testObj::test<5>(void)
{
  check(SeverityLevel::WARNING, "warning");
}

// check problem
template<>
template<>
void testObj::test<6>(void)
{
  check(SeverityLevel::PROBLEM, "problem");
}

// check error
template<>
template<>
void testObj::test<7>(void)
{
  check(SeverityLevel::ERROR, "error");
}

// check critical
template<>
template<>
void testObj::test<8>(void)
{
  check(SeverityLevel::CRITICAL, "critical");
}

} // namespace tut
