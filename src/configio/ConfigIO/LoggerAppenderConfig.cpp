/*
 * LoggerAppenderConfig.cpp
 *
 */

#include "ConfigIO/LoggerAppenderConfig.hpp"

namespace ConfigIO
{

LoggerAppenderConfig::LoggerAppenderConfig(const std::string &type,
                                           const std::string &name,
                                           const Options     &opts):
  type_(type),
  name_(name),
  opts_(opts)
{
  // TODO - test specific options for given type
}

const std::string &LoggerAppenderConfig::operator[](const std::string &name) const
{
  const Options::const_iterator it=opts_.find(name);
  if( it==opts_.end() )
    throw ExceptionInvalidOptionName("LoggerAppenderConfig::operator[]()", name);
  return it->second;
}

} // namespace ConfigIO
