/*
 * EnsureNewFile.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_ENSURENEWFILE_HPP_FILE
#define INCLUDE_TRIGGER_FILE_ENSURENEWFILE_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "System/AutoDescriptor.hpp"
#include "Trigger/File/ExceptionCannotOpenFile.hpp"

namespace Trigger
{
namespace File
{

/** \brief opens file, check if it is regular file and holds opened descriptor to it.
 *
 *  file descriptor is released in d-tor.
 */
class EnsureNewFile: private boost::noncopyable
{
public:
  /** \brief create new file.
   *  \param path path to file.
   */
  explicit EnsureNewFile(const std::string &path);

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
}; // class EnsureNewFile

} // namespace File
} // namespace Trigger

#endif
