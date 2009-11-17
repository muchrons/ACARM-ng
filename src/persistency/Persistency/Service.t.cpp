/*
 * Service.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Service.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestImpl: public Service
{
  TestImpl(const char *name, const uint16_t port, const char *protocol):
    Service(name, port, protocol)
  {
  }
}; // struct TestImpl

struct ServiceTestClass
{
};

typedef ServiceTestClass TestClass;
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
  const TestImpl ti("mail daemon", 25, "smtp");
  ensure_equals("invalid name",    ti.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    ti.getPort(),           25                    );
  ensure_equals("invalid service", ti.getProtocol().get(), string("smtp")        );
}

// test NULL protocol
template<>
template<>
void testObj::test<2>(void)
{
  const TestImpl ti("mail daemon", 25, NULL);
  ensure_equals("invalid name",    ti.getName().get(),     string("mail daemon") );
  ensure_equals("invalid port",    ti.getPort(),           25                    );
  ensure("invalid service",        ti.getProtocol().get()==NULL                  );
}

} // namespace tut
