/*
 * Process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_PROCESS_HPP_FILE

/* public header */

#include "Persistency/Process.hpp"
#include "Persistency/Stubs/ReferenceURL.hpp"

namespace Persistency
{
namespace Stubs
{
/** \brief process data repreesntation.
 */
class Process: public Persistency::Process
{
public:
  /** \brief creates object from given data.
   *  \param path      path to executable
   *  \param name      process name.
   *  \param md5       hash for given binary, if available.
   *  \param pid       proces number (ID).
   *  \param uid       user ID.
   *  \param username  name of user that called process.
   *  \param arguments arguments to the application when running.
   *  \param url       reference URL.
   */
  Process(const Path        &path,
          const Name        &name,
          const MD5Sum      *md5,
          const pid_t       *pid,
          const int         *uid,
          const Username    &username,
          const std::string *arguments,
          ReferenceURLPtr    url):
    Persistency::Process(path, name, md5, pid, uid, username, arguments, url)
  {
  }

  /** \brief create sample process.
   */
  Process(void):
    Persistency::Process("/a/b/c",
                         "process1",
                         NULL,
                         NULL,
                         NULL,
                         "john",
                         NULL,
                         ReferenceURLPtr(new ReferenceURL) )
  {
  }
}; // class Process

} // namespace Stubs
} // namespace Persistency

#endif
