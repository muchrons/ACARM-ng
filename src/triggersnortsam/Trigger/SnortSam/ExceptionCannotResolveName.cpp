/*
 * ExceptionCannotResolveName.cpp
 *
 */
#include "Trigger/SnortSam/ExceptionCannotResolveName.hpp"

namespace Trigger
{
namespace SnortSam
{

ExceptionCannotResolveName::ExceptionCannotResolveName(const Location    &where,
                                                       const std::string &host,
                                                       const std::string &details):
  Exception(where, cc("cannot resolve host name '", host, "': ", details))
{
}

} // namespace SnortSam
} // namespace Trigger
