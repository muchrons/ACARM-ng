/*
 * exports.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/exportBaseWrapper.hpp"

// export
BOOST_PYTHON_MODULE(trigger)
{
  Trigger::Python::exportBaseWrapper();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("trigger", inittrigger);
// TODO: dead code
//Trigger::Environment::StaticImporter g_import("trigger", PyInit_trigger);
} // unnamed namespace
