/*
 * ExceptionHandle.hpp
 *
 * this implementation is based on the text from Joseph Turner's blog:
 * http://thejosephturner.com/blog/2011/06/15/embedding-python-in-c-applications-with-boostpython-part-2/
 * good job Joseph! :-)
 *
 */
#include <sstream>
#include <cstdlib>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/ExceptionHandle.hpp"


using namespace std;
namespace py = boost::python;


namespace PythonAPI
{

ExceptionHandle::NoException::NoException(const Location &where):
  Exception(where, "no exception has been thrown from Python code")
{
}

ExceptionHandle::ExceptionHandle(void):
  type_("???"),
  msg_("???"),
  backtrace_("???")
{
  GlobalLock lock;
  // maby there was no exception at all?
  if( PyErr_Occurred()==NULL )
    throw NoException(SYSTEM_SAVE_LOCATION);

  // read exception-related data from python
  PyObject *typePtr     =NULL;
  PyObject *valuePtr    =NULL;
  PyObject *backtracePtr=NULL;
  PyErr_Fetch(&typePtr, &valuePtr, &backtracePtr);

  // check if type is set
  if(typePtr!=NULL)
  {
    py::handle<>        h(typePtr);
    py::str             pyStr(h);
    py::extract<string> extrPyStr(pyStr);
    if( extrPyStr.check() )
      type_=extrPyStr();
  }
  // check if message is set
  if(valuePtr!=NULL)
  {
    py::handle<>        h(valuePtr);
    py::str             pyStr(h);
    py::extract<string> extrPyStr(pyStr);
    if( extrPyStr.check() )
      msg_=extrPyStr();
  }
  // check if backtrace is set
  if(backtracePtr!=NULL)
  {
    py::handle<>        h(backtracePtr);
    py::object          tb( py::import("traceback") );
    py::object          formatTb( tb.attr("format_tb") );
    py::object          tbList( formatTb(h) );
    py::object          tbStr( py::str("\n").join(tbList) );
    py::extract<string> extrPyStr(tbStr);
    if( extrPyStr.check() )
      backtrace_=extrPyStr();
  }

  // ok - this one has been read
  clearState();
}

void ExceptionHandle::rethrow(void) const
{
  throw ExceptionFromScript(SYSTEM_SAVE_LOCATION, getType(), getMessage(), getBacktraceStr());
}

std::string ExceptionHandle::str(void) const
{
  ExceptionFromScript ex(SYSTEM_SAVE_LOCATION, getType(), getMessage(), getBacktraceStr());
  return ex.what();
}

void ExceptionHandle::clearState(void)
{
  GlobalLock lock;
  PyErr_Clear();
}

} // namespace PythonAPI
