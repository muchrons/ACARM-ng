/*
 * Process.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PROCESS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_PROCESS_T_HPP_FILE

#include "Persistency/Process.hpp"
#include "Persistency/ReferenceURL.t.hpp"

namespace
{

struct ProcessTestImpl: public Persistency::Process
{
  typedef Persistency::Process         Process;
  typedef Persistency::ProcessPtr      ProcessPtr;
  typedef Persistency::ReferenceURLPtr ReferenceURLPtr;
  typedef Persistency::MD5Sum          MD5Sum;

  ProcessTestImpl(const char              *path,
                  const char              *name,
                  const MD5Sum            *md5,
                  const pid_t             *pid,
                  const int               *uid,
                  const Process::Username  username,
                  const std::string       *arguments,
                  ReferenceURLPtr          url):
    Process(path, name, md5, pid, uid, username, arguments, url)
  {
  }

  static ProcessPtr makeNew(const char *name="process name")
  {
    ProcessPtr ptr( new ProcessTestImpl( "/path/to/bin",
                                         name,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         ReferenceURLPtr() ) );
    return ptr;
  }
}; // struct ProcessTestImpl

} // unnamed namespace

#endif
