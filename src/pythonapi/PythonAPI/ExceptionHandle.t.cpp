/*
 * ExceptionHandle.t.cpp
 *
 */
#include <tut.h>
#include <stdexcept>
#include <boost/python.hpp>

#include "PythonAPI/ExceptionHandle.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace boost::python;
using namespace PythonAPI;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    // initialize all data members
    Py_Initialize();
    mainModule_   =import("__main__");
    mainNamespace_=mainModule_.attr("__dict__");
  }

  Environment env_;     // required to import all required modules
  object      mainModule_;
  object      mainNamespace_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/ExceptionHandle");
} // unnamed namespace

namespace tut
{

// test error message rethrowing
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    exec("some invalid python code", mainNamespace_);
  }
  catch(const boost::python::error_already_set &)
  {
    PythonAPI::ExceptionHandle ex;
    try
    {
      ex.rethrow(); // should throw exception genereted from Python
      fail("rethrow() didn't throw");
    }
    catch(const ExceptionFromScript &)
    {
      // this is expected
    }
  }
}

// test error when no exception has been risen
template<>
template<>
void testObj::test<2>(void)
{
  ExceptionHandle::clearState();
  try
  {
    ExceptionHandle ex; // should throw
    fail("didn't throw on no exception from Python");
  }
  catch(const ExceptionHandle::NoException &)
  {
    // this is expected
  }
}

// test partial error strings
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    exec("some invalid python code", mainNamespace_);
  }
  catch(const boost::python::error_already_set &)
  {
    PythonAPI::ExceptionHandle ex;
    ensure("invalid string", ex.str().length()>3*4+4 ); // minimal, reasonable length
    ensure("invalid type", ex.getType().length()>=3 );
    ensure("invalid message", ex.getMessage().length()>=3 );
    ensure("invalid backtrace string", ex.getBacktraceStr().length()>=3 );
  }
}

} // namespace tut
