/*
 * exports.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/exportTriggerBaseWrapper.hpp"

// export
BOOST_PYTHON_MODULE(triggerapi)
{
  Trigger::Python::exportTriggerBaseWrapper();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("triggerapi", inittriggerapi);
//Trigger::Environment::StaticImporter g_import("triggerapi", PyInit_triggerapi);
} // unnamed namespace
