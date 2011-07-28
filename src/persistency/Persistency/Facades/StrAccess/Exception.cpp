/*
 * Exception.cpp
 *
 */
#include "Persistency/Facades/StrAccess/Exception.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

Exception::Exception(const Location &where, const std::string &details):
  Persistency::Exception(where, cc("string-access facade error: ", details) )
{
}

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency
