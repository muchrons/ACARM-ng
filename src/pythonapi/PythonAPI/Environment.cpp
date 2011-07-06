/*
 * Environment.cpp
 *
 */
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/ExceptionHandle.hpp"

namespace py=boost::python;


namespace PythonAPI
{

Environment::Environment(void)
{
  Py_Initialize();    // may be called multiple times
  mainModule_   =py::import("__main__");
  mainNamespace_=mainModule_.attr("__dict__");
}

void Environment::run(const std::string &script)
{
  try
  {
    ExceptionHandle::clearState();
    py::exec(script.c_str(), mainNamespace_);
  }
  catch(const boost::python::error_already_set &)
  {
    ExceptionHandle ex;
    ex.rethrow();
  }
}

void Environment::importModule(const std::string &module, ModuleInitFunc init)
{
  if(init==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "init function cannot be NULL");

  if( PyImport_AppendInittab(module.c_str(), init)!=0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "PyImport_AppendInittab() failed; unable to init module "+module);
}

} // namespace PythonAPI
