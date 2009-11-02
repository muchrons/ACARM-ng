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

} // namespace Appenders
} // namespace Logger
