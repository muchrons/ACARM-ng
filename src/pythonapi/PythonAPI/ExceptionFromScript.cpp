/*
 * ExceptionFromScript.cpp
 *
 */
#include "PythonAPI/ExceptionFromScript.hpp"

namespace PythonAPI
{

ExceptionFromScript::ExceptionFromScript(const Location    &where,
                                         const std::string &type,
                                         const std::string &msg,
                                         const std::string &backtrace):
  Exception(where, cc(type, ": ", msg, ";\nbacktrace is:\n", backtrace) )
{
}

} // namespace PythonAPI
