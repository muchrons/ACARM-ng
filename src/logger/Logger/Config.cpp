/*
 * Config.cpp
 *
 */
#include "Logger/Config.hpp"
#include "Logger/detail/ConfigSetSingleton.hpp"

namespace Logger
{

void Config::reinitAppenders(void)
{
  // forward this call
  detail::ConfigSetSingleton::get()->reinitAppenders();
}

} // namespace Logger
