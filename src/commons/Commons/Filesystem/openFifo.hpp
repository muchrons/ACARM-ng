/*
 * openFifo.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_OPENFIFO_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_OPENFIFO_HPP_FILE

/* public header */

#include <fstream>

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/Mode.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief opens fifo in a given mode.
 *  \param p    path to fifo.
 *  \param mode opening mode.
 *  \return open output stream to a fifo. stream is never NULL.
 *  \note always use cannonicalized path to open fifo.
 *
 *  opens given fifo, performing proper sanity-checks before doing so.
 *  throws on error.
 */
SharedPtrNotNULL<std::fstream> openFifo(const Base::Filesystem::Path &p, const Mode mode=Mode::READWRITE);

} // namespace Filesystem
} // namespace Commons

#endif
