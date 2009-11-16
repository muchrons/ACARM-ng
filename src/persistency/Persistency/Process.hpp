/*
 * Process.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_PROCESS_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/MD5Sum.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"

namespace Persistency
{

class Process: private boost::noncopyable
{
public:
  typedef detail::LimitedNULLString<256> Path;
  typedef detail::LimitedString<256>     Name;

  virtual ~Process(void);

  const Path &getPath(void) const;
  const Name &getName(void) const;
  const MD5Sum *getMD5(void) const;

protected:
  Process(const Path   &path,
          const Name   &name,
          const MD5Sum *md5=NULL);

private:
  Path                      path_;
  Name                      name_;
  boost::scoped_ptr<MD5Sum> md5_;
}; // class Process

} // namespace Persistency

#endif
