/*
 * createFifo.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEFIFO_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEFIFO_HPP_FILE

/* public header */

#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new fifo queue.
 *  \param p path to fifo to be created.
 *  \return open output stream to fifo. returned value is never NULL.
 *  \note always use cannonicalized path to open fifo!
 *
 *  creates given fifo, performing proper sanity-checks before doing so.
 *  throws on error.
 */
SharedPtrNotNULL<std::fstream> createFifo(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
