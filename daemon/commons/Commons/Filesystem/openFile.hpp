/*
 * openFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_OPENFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_OPENFILE_HPP_FILE

/* public header */

#include <memory>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/Filesystem/Mode.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief opens file in a given mode.
 *  \param p    path to file.
 *  \param mode opening mode.
 *  \return open output stream to a file.
 *  \note always use cannonicalized path to open file!
 *
 *  opens given file, performing proper sanity-checks before doing so.
 */
std::auto_ptr<std::fstream> openFile(const boost::filesystem::path &p, const Mode mode=Mode::READWRITE);

} // namespace Filesystem
} // namespace Commons

#endif
