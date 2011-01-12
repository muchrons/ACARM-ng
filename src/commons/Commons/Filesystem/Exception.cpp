/*
 * Exception.cpp
 *
 */
#include "Commons/Filesystem/Exception.hpp"

namespace Commons
{
namespace Filesystem
{

Exception::Exception(const Location &where, const std::string &what):
  Commons::Exception(where, what)
{
}

} // namespace Filesystem
} // namespace Commons
