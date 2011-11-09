/*
 * isFileSane.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_ISFILESANE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_ISFILESANE_HPP_FILE

/* public header */

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief check if path does not contain any suspicious elements.
 *  \param p path to file.
 *  \return false if path is suspisious, true otherwise.
 *
 *  call checks if non of path elements contains symbolic links, nor hardlinks.
 */
bool isFileSane(const Base::Filesystem::Path &p);

} // namespace Filesystem
} // namespace Commons

#endif
