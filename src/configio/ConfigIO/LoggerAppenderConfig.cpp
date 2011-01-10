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
  // note: no validation is performed on this level. it is up to logger library
  //       to interpret values provided as options.
}

const LoggerAppenderConfig::ParameterValues &
            LoggerAppenderConfig::operator[](const std::string &name) const
{
  const Options::const_iterator it=opts_.find(name);
  if( it==opts_.end() )
    throw ExceptionInvalidOptionName(SYSTEM_SAVE_LOCATION, name);
  return it->second;
}

} // namespace ConfigIO
