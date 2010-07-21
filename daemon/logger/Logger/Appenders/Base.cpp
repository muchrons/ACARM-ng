/*
 * Base.cpp
 *
 */
#include <cassert>

#include "Logger/Appenders/Base.hpp"

namespace Logger
{
namespace Appenders
{

namespace
{
bool isLocked(::Base::Threads::Mutex &m)
{
  const bool locked=m.try_lock();
  if(locked)
    m.unlock();
  return !locked;
}
} // unnamed namespace

Base::~Base(void)
{
}

void Base::append(const std::string &str)
{
  ::Base::Threads::Lock lock(mutex_);
  appendImpl(str);
}

void Base::reinit(void)
{
  ::Base::Threads::Lock lock(mutex_);
  reinitAlreadyLocked();
}

const char *Base::getTypeName(void) const
{
  const char *name=getTypeNameImpl();
  assert(name!=NULL);
  return name;
}

void Base::reinitAlreadyLocked(void)
{
  assert( isLocked(mutex_) && "mutex is NOT locked - you probably wanted to run reinit() here" );
  reinitImpl();
}

} // namespace Appenders
} // namespace Logger
