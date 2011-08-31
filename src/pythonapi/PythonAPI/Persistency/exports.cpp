/*
 * exports.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/Persistency/exportOptionalString.hpp"
#include "PythonAPI/Persistency/exportMetaAlert.hpp"

// export
BOOST_PYTHON_MODULE(persistency)
{
  PythonAPI::Persistency::exportOptionalString();
  PythonAPI::Persistency::exportMetaAlert();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("persistency", initpersistency);
//PythonAPI::Environment::StaticImporter g_import("persistency", PyInit_persistency);
} // unnamed namespace
