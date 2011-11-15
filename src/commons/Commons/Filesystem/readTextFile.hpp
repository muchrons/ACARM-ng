/*
 * readTextFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_READTEXTFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_READTEXTFILE_HPP_FILE

/* public header */

#include <boost/shared_array.hpp>

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/openFile.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief opens file for reading and returns its content as string.
 *  \param p    path to file.
 *  \return file's content as an array. returned value is always non-NULL!
 *  \note always use cannonicalized path to open file.
 */
boost::shared_array<char> readTextFile(const Base::Filesystem::Path &p);

} // namespace Filesystem
} // namespace Commons

#endif
