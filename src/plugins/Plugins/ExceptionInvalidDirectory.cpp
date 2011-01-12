/*
 * ExceptionInvalidDirectory.cpp
 *
 */
#include "Plugins/ExceptionInvalidDirectory.hpp"

namespace Plugins
{

ExceptionInvalidDirectory::ExceptionInvalidDirectory(const Location                &where,
                                                     const boost::filesystem::path &dir):
  Exception(where, cc("invalid plugins' directory: '", dir, "'") )
{
}

} // namespace Plugins
