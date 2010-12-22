/*
 * ExceptionRegistrationError.cpp
 *
 */
#include "Plugins/ExceptionRegistrationError.hpp"

namespace Plugins
{

ExceptionRegistrationError::ExceptionRegistrationError(const Location    &where,
                                                       const std::string &name,
                                                       const std::string &msg):
  Exception(where, cc("unable to register factory '", name, "': ", msg) )
{
}

} // namespace Plugins
