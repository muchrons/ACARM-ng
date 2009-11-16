/*
 * Process.cpp
 *
 */

#include "Persistency/Process.hpp"

namespace Persistency
{

Process::~Process(void)
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

Process::Process(const Path   &path,
                 const Name   &name,
                 const MD5Sum *md5):
  path_(path),
  name_(name),
  md5_( (md5!=NULL)?(new MD5Sum(*md5)):NULL )
{
}

} // namespace Persistency
