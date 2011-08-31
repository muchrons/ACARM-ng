/*
 * exports.cpp
 *
 */
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/exports.hpp"

namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("trigger", inittrigger);
//Trigger::Environment::StaticImporter g_import("trigger", PyInit_trigger);
} // unnamed namespace
