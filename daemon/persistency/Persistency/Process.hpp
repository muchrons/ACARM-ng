/*
 * Process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_PROCESS_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/MD5Sum.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/detail/NullValue.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"

namespace Persistency
{

/** \brief process data repreesntation.
 */
class Process: private boost::noncopyable
{
public:
  /** \brief path to file type.
   */
  typedef detail::LimitedNULLString<256> Path;
  /** \brief process name representation.
   */
  typedef detail::LimitedString<256>     Name;
  /** \brief user name type.
   */
  typedef detail::LimitedNULLString<32>  Username;

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

  /** \brief gets path to executable.
   *  \return path to executable process has been run from.
   */
  const Path &getPath(void) const;
  /** \brief gets process name.
   *  \return process name.
   */
  const Name &getName(void) const;
  /** \brief MD5 hash of given file.
   *  \return hash of executable or NULL if not present.
   */
  const MD5Sum *getMD5(void) const;
  /** \brief get process ID.
   *  \return pid.
   */
  const pid_t *getPID(void) const;
  /** \brief gets UID.
   *  \return UID.
   */
  const int *getUID(void) const;
  /** \brief get username of caller.
   *  \return username owning process.
   */
  const Username &getUsername(void) const;
  /** \brief gets arguments passed to application.
   *  \return process parameters
   */
  const std::string *getParameters(void) const;
  /** \brief gets reference URL.
   *  \return referenceu url for a given process.
   */
  const ReferenceURL *getReferenceURL(void) const;

private:
  Path                           path_;
  Name                           name_;
  boost::scoped_ptr<MD5Sum>      md5_;
  detail::NullValue<pid_t>       pid_;
  detail::NullValue<int>         uid_;
  Username                       username_;
  boost::scoped_ptr<std::string> arguments_;
  ReferenceURLPtr                url_;
}; // class Process


/** \brief smart pointer to process type. */
typedef boost::shared_ptr<Process>         ProcessPtr;

/** \brief smart pointer to process type, checked not to be NULL. */
typedef Commons::SharedPtrNotNULL<Process> ProcessPtrNN;

} // namespace Persistency

#endif
