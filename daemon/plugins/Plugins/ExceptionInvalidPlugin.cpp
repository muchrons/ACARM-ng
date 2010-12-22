/*
 * ExceptionInvalidPlugin.cpp
 *
 */
#include "Plugins/ExceptionInvalidPlugin.hpp"

namespace Plugins
{

ExceptionInvalidPlugin::ExceptionInvalidPlugin(const Location    &where,
                                               const std::string &msg):
  Exception(where, cc("invalid plugin; error is: ", msg) )
{
}

} // namespace Plugins
