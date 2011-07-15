/*
 * Exception.cpp
 *
 */
#include "PythonAPI/Exception.hpp"

namespace PythonAPI
{

Exception::Exception(const Location &where, const std::string &msg):
  Commons::Exception(where, cc("PythonAPI exception: ", msg) )
{
}

} // namespace PythonAPI
