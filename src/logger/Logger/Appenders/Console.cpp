/*
 * Console.cpp
 *
 */
#include <iostream>

#include "Logger/Appenders/Console.hpp"

namespace Logger
{
namespace Appenders
{

void Console::appendImpl(const std::string &str)
{
  std::cout<<str<<std::endl;
}

const char *Console::getTypeNameImpl(void) const
{
  return Console::getThisTypeName();
}

} // namespace Appenders
} // namespace Logger
