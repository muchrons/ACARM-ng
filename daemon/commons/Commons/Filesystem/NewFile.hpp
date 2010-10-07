/*
 * NewFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_ENSURENEWFILESYSTEM_HPP_FILESYSTEM
#define INCLUDE_COMMONS_FILESYSTEM_ENSURENEWFILESYSTEM_HPP_FILESYSTEM

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "System/AutoDescriptor.hpp"
#include "Commons/Filesystem/Path.hpp"
#include "Commons/Filesystem/ExceptionFileIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief opens file, check if it is regular file and holds opened descriptor to it.
 *
 *  file descriptor is released in d-tor.
 */
class NewFile: private boost::noncopyable
{
public:
  /** \brief create new file.
   *  \param path path to file.
   */
  explicit NewFile(const Path &path);

  /** \brief gives access to file descriptor.
   *  \return descriptor to opened file.
   */
  int get(void) const
  {
    return fd_.get();
  }

private:
  Logger::Node           log_;
  System::AutoDescriptor fd_;
}; // class NewFile

} // namespace Filesystem
} // namespace Commons

#endif
