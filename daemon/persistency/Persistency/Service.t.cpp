/*
 * Service.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Service.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
  TestClass(void):
    custom_("mail daemon", 25, "smtp", makeNewReferenceURL() )
  {
  }

  const Service custom_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Service");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name",    custom_.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    custom_.getPort(),           25                    );
  ensure_equals("invalid service", custom_.getProtocol().get(), string("smtp")        );
  ensure("invalid url",            custom_.getReferenceURL()!=NULL);
}

// test NULL protocol
template<>
template<>
void testObj::test<2>(void)
{
  const Service ti("mail daemon", 25, NULL, makeNewReferenceURL() );
  ensure_equals("invalid name",    ti.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    ti.getPort(),           25                    );
  ensure("invalid service",        ti.getProtocol().get()==NULL                  );
  ensure("invalid url",            ti.getReferenceURL()!=NULL);
}

// test NULL reference URL
template<>
template<>
void testObj::test<3>(void)
{
  const Service ti("mail daemon", 25, "smtp", ReferenceURLPtr() );
  ensure_equals("invalid name",    ti.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    ti.getPort(),           25                    );
  ensure_equals("invalid service", ti.getProtocol().get(), string("smtp")        );
  ensure("invalid url",            ti.getReferenceURL()==NULL);
}

// test comaprison
template<>
template<>
void testObj::test<4>(void)
{
  const Service ti("mail daemon", 25, "smtp", makeNewReferenceURL() );
  ensure("instances differ", custom_==ti);
}

// test comaprison when name differs
template<>
template<>
void testObj::test<5>(void)
{
  const Service ti("different mail daemon", 25, "smtp", makeNewReferenceURL() );
  TestHelpers::checkEquality(custom_, ti);
}

// test comaprison when port differs
template<>
template<>
void testObj::test<6>(void)
{
  const Service ti("mail daemon", 42, "smtp", makeNewReferenceURL() );
  TestHelpers::checkEquality(custom_, ti);
}

// test comaprison when protocol differs
template<>
template<>
void testObj::test<7>(void)
{
  const Service ti("mail daemon", 25, "narf", makeNewReferenceURL() );
  TestHelpers::checkEquality(custom_, ti);
}

// test comaprison when reference URL differs
template<>
template<>
void testObj::test<8>(void)
{
  const Service ti( "mail daemon", 25, "smtp",
                    makeNewReferenceURL("http://different.net") );
  TestHelpers::checkEquality(custom_, ti);
}

} // namespace tut
