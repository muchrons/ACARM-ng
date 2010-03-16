/*
 * Location.cpp
 *
 */
#include <sstream>

#include "System/Exceptions/detail/Location.hpp"

namespace System
{
namespace Exceptions
{
namespace detail
{

namespace
{
inline std::string formatLocation(const char   *file,
                                  unsigned int  line,
                                  const char   *function)
{
  std::stringstream ss;
  ss<<file<<":"<<line<<"@"<<function;
  return ss.str();
} // formatLocation()
} // unnamed namespace

Location::Location(const char   *file,
                   unsigned int  line,
                   const char   *function):
  str_( formatLocation(file, line, function) )
{
}

} // namespace detail
} // namespace Exceptions
} // namespace System
