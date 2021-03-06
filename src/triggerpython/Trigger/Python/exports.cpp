/*
 * exports.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/exportBaseWrapper.hpp"

// export
BOOST_PYTHON_MODULE(triggerapi)
{
  Trigger::Python::exportBaseWrapper();
}

// register
namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("triggerapi", inittriggerapi);
} // unnamed namespace
