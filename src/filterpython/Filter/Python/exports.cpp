/*
 * exports.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/exportFilterBaseWrapper.hpp"
#include "Filter/Python/exportDataWrapper.hpp"

// export
BOOST_PYTHON_MODULE(filterapi)
{
  Filter::Python::exportFilterBaseWrapper();
  Filter::Python::exportDataWrapper();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("filterapi", initfilterapi);
//Filter::Environment::StaticImporter g_import("filterapi", PyInit_filterapi);
} // unnamed namespace
