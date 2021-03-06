/*
 * openElement.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_OPENELEMENT_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_OPENELEMENT_HPP_FILE

#include <fstream>

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/Mode.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief opens element in a given mode.
 *  \param p      path to element.
 *  \param mode   opening mode.
 *  \param isSane sanity-checking functoon.
 *  \return open output stream to an element. stream is never NULL.
 *
 *  \note always use cannonicalized path to open elements.
 *
 *  \warning this call does nearly non checks, except for user-provided
 *           isSane call, this it must remain private, implementation call.
 *
 *  opens given element, performing proper sanity-checks (via user-provided
 *  function) before doing so. throws on error.
 */
SharedPtrNotNULL<std::fstream> openElement(const Base::Filesystem::Path &p,
                                           const Mode                    mode,
                                           bool (*isSane)(const boost::filesystem::path &) );

} // namespace Filesystem
} // namespace Commons

#endif
