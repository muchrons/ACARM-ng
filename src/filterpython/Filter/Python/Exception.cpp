/*
 * Exception.cpp
 *
 */
#include "Filter/Python/Exception.hpp"

namespace Filter
{
namespace Python
{

Exception::Exception(const Location &where, const std::string &what):
  Filter::Exception(where, "python", what)
{
}

} // namespace Python
} // namespace Filter
