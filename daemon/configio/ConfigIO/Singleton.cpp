/*
 * Singleton.cpp
 *
 */
#include <cassert>

#include "ConfigIO/Singleton.hpp"
#include "ConfigIO/Parser.hpp"

namespace ConfigIO
{

SingletonImpl::~SingletonImpl(void)
{
}

void SingletonImpl::rereadConfig(const char *path)
{
  // try reading config file - either default or the one passed
  // as an argument.
  boost::scoped_ptr<Parser> tmp( path?new Parser(path):new Parser );
  // if reading succeded (i.e. no exception has been thrown make new
  // configuration persistent.
  p_.swap(tmp);
  assert( p_.get()!=NULL );
}

const PersistencyConfig &SingletonImpl::persistencyConfig(void) const
{
  assert( p_.get()!=NULL );
  return p_->getPersistencyConfig();
}

const LoggerConfig &SingletonImpl::loggerConfig(void) const
{
  assert( p_.get()!=NULL );
  return p_->getLoggerConfig();
}

const FiltersConfigCollection &SingletonImpl::filtersConfig(void) const
{
  assert( p_.get()!=NULL );
  return p_->getFiltersConfig();
}

const TriggersConfigCollection &SingletonImpl::triggersConfig(void) const
{
  assert( p_.get()!=NULL );
  return p_->getTriggersConfig();
}

SingletonImpl::SingletonImpl(void):
  p_(new Parser)
{
  assert( p_.get()!=NULL );
}

} // namespace ConfigIO
