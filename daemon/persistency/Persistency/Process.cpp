/*
 * Process.cpp
 *
 */

#include "Persistency/Process.hpp"

using namespace std;


namespace Persistency
{

Process::Process(const Process::Path     &path,
                 const Process::Name     &name,
                 const MD5Sum            *md5,
                 const pid_t             *pid,
                 const int               *uid,
                 const Process::Username &username,
                 const std::string       *arguments,
                 ReferenceURLPtr          url):
  path_(path),
  name_(name),
  md5_( (md5!=NULL)?(new MD5Sum(*md5)):NULL ),
  pid_(pid),
  uid_(uid),
  username_(username),
  arguments_( (arguments!=NULL)?(new string(*arguments)):NULL ),
  url_(url)
{
}

const Process::Path &Process::getPath(void) const
{
  return path_;
}

const Process::Name &Process::getName(void) const
{
  return name_;
}

const MD5Sum *Process::getMD5(void) const
{
  return md5_.get();
}

const pid_t *Process::getPID(void) const
{
  return pid_.get();
}

const int *Process::getUID(void) const
{
  return uid_.get();
}

const Process::Username &Process::getUsername(void) const
{
  return username_;
}

const std::string *Process::getParameters(void) const
{
  return arguments_.get();
}

const ReferenceURL *Process::getReferenceURL(void) const
{
  return url_.get();
}

} // namespace Persistency
