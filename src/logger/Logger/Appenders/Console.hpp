/*
 * Console.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_CONSOLE_HPP_FILE

#include <iostream>

#include "Logger/Appenders/Base.hpp"

// TODO: comment
// TODO: test

namespace Logger
{
namespace Appenders
{

class Console: public Base
{
private:
  virtual void appendImpl(const std::string &str)
  {
    std::cout<<str;
  }
}; // class Console

} // namespace Appenders
} // namespace Logger

#endif
