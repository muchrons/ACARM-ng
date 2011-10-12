/*
 * Exception.cpp
 *
 */
#include "DataFacades/StrAccess/Exception.hpp"

namespace Facades
{
namespace StrAccess
{

Exception::Exception(const Location &where, const std::string &details):
  DataFacades::Exception(where, cc("string-access facade error: ", details) )
{
}

} // namespace StrAccess
} // namespace Facades
