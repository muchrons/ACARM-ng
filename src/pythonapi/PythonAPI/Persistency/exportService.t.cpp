/*
 * exportService.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/Service.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace ::Persistency;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    env_.importModule("persistency");
    //env_.run("from persistency import *");

    env_.run("tmp=persistency.ReferenceURL( persistency.ReferenceURL.Name(\"un\"),"
                                           "persistency.ReferenceURL.URL(\"link\") ) ");
    env_.run("tmp=persistency.Service( "
                    "persistency.Service.Name(\"secure shell\"),"
                    "persistency.PortNumber(22),"
                    "persistency.Service.Protocol(\"ssh\"),"
                    "tmp)");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportService");
} // unnamed namespace

namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.getName()");
  const Service::Name &ret=env_.var<const Service::Name&>("ret");
  ensure_equals("invalid value returned", ret.get(), string("secure shell") );
}

// test getting reference url
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.getReferenceURL()");
  ConstReferenceURLPtr ret=env_.var<ConstReferenceURLPtr>("ret");
  ensure("pointer is NULL", ret.get()!=NULL);
}

// test getting protocol
template<>
template<>
void testObj::test<3>(void)
{
  env_.run("ret=tmp.getProtocol()");
  const Service::Protocol &ret=env_.var<const Service::Protocol&>("ret");
  ensure_equals("invalid value returned", ret.get(), string("secure shell") );
}

// test getting port number
template<>
template<>
void testObj::test<4>(void)
{
  env_.run("ret=tmp.getPort()");
  const PortNumber ret=env_.var<PortNumber>("ret");
  ensure_equals("invalid value returned", ret.get(), 22);
}

} // namespace tut
