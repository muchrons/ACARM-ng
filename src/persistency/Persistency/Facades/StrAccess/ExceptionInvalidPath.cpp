/*
 * ExceptionInvalidPath.cpp
 *
 */
#include "Persistency/Facades/StrAccess/ExceptionInvalidPath.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

ExceptionInvalidPath::ExceptionInvalidPath(const Location    &where,
                                           const std::string &path,
                                           const std::string &near):
  Exception(where, cc("invalid path '", path, "' near '", near, "' token") )
{
}

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency
