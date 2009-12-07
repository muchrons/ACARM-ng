/*
 * Service.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Service.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
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
  const Service ti("mail daemon", 25, "smtp", makeNewReferenceURL() );
  ensure_equals("invalid name",    ti.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    ti.getPort(),           25                    );
  ensure_equals("invalid service", ti.getProtocol().get(), string("smtp")        );
  ensure("invalid url",            ti.getReferenceURL()!=NULL);
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

} // namespace tut
