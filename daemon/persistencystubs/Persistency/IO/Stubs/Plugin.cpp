/*
 * Plugin.cpp
 *
 */
#include "Plugins/c_helper.hpp"
#include "Persistency/IO/Stubs/ConnectionBuilder.hpp"

using namespace Persistency::IO;
using namespace Persistency::IO::Stubs;

extern "C" const char *register_plugin(void *dynObj)
{
  return Plugins::c_helper<BackendFactory, ConnectionBuilder>(dynObj);
}
