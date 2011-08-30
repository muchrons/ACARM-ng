/*
 * exports.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Environment.hpp"
#include "PythonAPI/Persistency/exports.hpp"

namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("persistency", initpersistency);
//PythonAPI::Environment::StaticImporter g_import("persistency", PyInit_persistency);
} // unnamed namespace
