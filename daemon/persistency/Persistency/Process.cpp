/*
 * Process.cpp
 *
 */
#include "Base/ViaPointer.hpp"
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
                 const char              *arguments,
                 ConstReferenceURLPtr     url):
  path_(path),
  name_(name),
  md5_( (md5!=NULL)?(new MD5Sum(*md5)):NULL ),
  pid_(pid),
  uid_(uid),
  username_(username),
  arguments_( (arguments!=NULL)?(new string(arguments)):NULL ),
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

const char *Process::getParameters(void) const
{
  if( arguments_.get()==NULL )
    return NULL;
  return arguments_->c_str();
}

ConstReferenceURLPtr Process::getReferenceURL(void) const
{
  return url_;
}

bool Process::operator==(const Process &other) const
{
  if(this==&other)
    return true;

  if( getPath()!=other.getPath() )
    return false;
  if( getName()!=other.getName() )
    return false;
  if( !Base::ViaPointer::equal( getMD5(), other.getMD5() ) )
    return false;
  if( !Base::ViaPointer::equal( getPID(), other.getPID() ) )
    return false;
  if( !Base::ViaPointer::equal( getUID(), other.getUID() ) )
    return false;
  if( getUsername()!=other.getUsername() )
    return false;
  if( !Base::ViaPointer::equal( getParameters(), other.getParameters() ) )
    return false;
  if( !Base::ViaPointer::equal( getReferenceURL().get(), other.getReferenceURL().get() ) )
    return false;
  // if all fields are identical, Processes are identical too.
  return true;
}

} // namespace Persistency
