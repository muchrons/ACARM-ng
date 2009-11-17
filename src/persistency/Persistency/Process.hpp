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

#include "Persistency/MD5Sum.hpp"
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
  /** \brief ensure proper destruction when inherited.
   */
  virtual ~Process(void);
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

protected:
  /** \brief creates object from given data.
   *  \param path path to executable
   *  \param name process name.
   *  \param md5  hash for given binary, if available.
   */
  Process(const Path   &path,
          const Name   &name,
          const MD5Sum *md5=NULL);

private:
  Path                      path_;
  Name                      name_;
  boost::scoped_ptr<MD5Sum> md5_;
}; // class Process


/** \brief smart pointer to process type.
 */
typedef boost::shared_ptr<Process> ProcessPtr;

} // namespace Persistency

#endif
