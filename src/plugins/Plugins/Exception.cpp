/*
 * Exception.cpp
 *
 */
#include "Plugins/Exception.hpp"

namespace Plugins
{

Exception::Exception(const Location &where, const std::string &msg):
  Commons::Exception(where, msg)
{
}

} // namespace Plugins
