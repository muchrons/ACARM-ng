/*
 * Process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_PROCESS_HPP_FILE

#include "Persistency/Process.hpp"

namespace Persistency
{
namespace Postgres
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
          ReferenceURLPtr    url);
}; // class Process

} // namespace Postgres
} // namespace Persistency

#endif
