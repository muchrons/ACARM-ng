/*
 * exports.t.cpp
 *
 */
#include <tut.h>
#include <stdexcept>
#include <boost/python.hpp>

#include "PythonAPI/Persistency/exports.hpp"
#include "PythonAPI/ExceptionHandle.hpp"

using namespace std;
using namespace boost::python;
namespace py = boost::python;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    // initialize just once
    static bool initDone=false;
    if(initDone)
      return;

    // init module
    if( PyImport_AppendInittab("persistency", PyInit_persistency)!=0 )
      throw std::runtime_error("unable to load 'persistency' module");
    // initialize all data members
    Py_Initialize();
    mainModule_   =import("__main__");
    mainNamespace_=mainModule_.attr("__dict__");
  }

  void runTestScript(const char *script)
  {
    try
    {
      exec(script, mainNamespace_);
    }
    catch(const boost::python::error_already_set &)
    {
      PythonAPI::ExceptionHandle ex;
      ex.rethrow();
    }
  }

private:
  object mainModule_;
  object mainNamespace_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exports");
} // unnamed namespace

namespace tut
{

// test for Certainty class' presence
template<>
template<>
void testObj::test<1>(void)
{
  const char *script="c=persistency.Certainty(0.42)\n";
                     //"c.get()";
  runTestScript(script);
}

} // namespace tut
