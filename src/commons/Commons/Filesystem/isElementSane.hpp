/*
 * isElementSane.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_ISELEMENTSANE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_ISELEMENTSANE_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief check if given element is sane.
 *  \param p path to file.
 *  \return false if path is suspisious, true otherwise.
 *
 *  call checks if element's not symbolic link nor hard link (in case of files).
 */
bool isElementSane(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
