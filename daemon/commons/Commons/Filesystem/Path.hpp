/*
 * Path.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_PATH_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_PATH_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

namespace Commons
{
namespace Filesystem
{

/** \brief class repersenting path in the filesystem. */
typedef boost::filesystem::path Path;

} // namespace Filesystem
} // namespace Commons

#endif
