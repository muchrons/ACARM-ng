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

Console::Console(void):
  Stream<Console>( std::cout )
{
}

void Console::reinitImpl(void)
{
  std::cout.clear();    // removed error markers
}

} // namespace Appenders
} // namespace Logger
