/*
 * ExceptionInvalidPath.cpp
 *
 */
#include "DataFacades/StrAccess/ExceptionInvalidPath.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

ExceptionInvalidPath::ExceptionInvalidPath(const Location    &where,
                                           const std::string &path,
                                           const std::string &near,
                                           const std::string &details):
  Exception(where, cc("invalid path '", path, "' near '", near, "' token: ", details) )
{
}

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency
