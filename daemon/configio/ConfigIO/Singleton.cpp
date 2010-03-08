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

SingletonImpl::SingletonImpl(void):
  p_(new Parser)
{
}

} // namespace ConfigIO
