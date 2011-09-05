/*
 * exports.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/exportBaseWrapper.hpp"
#include "Filter/Python/exportDataWrapper.hpp"

// export
BOOST_PYTHON_MODULE(filter)
{
  Filter::Python::exportBaseWrapper();
  Filter::Python::exportDataWrapper();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("filter", initfilter);
//Filter::Environment::StaticImporter g_import("filter", PyInit_filter);
} // unnamed namespace
