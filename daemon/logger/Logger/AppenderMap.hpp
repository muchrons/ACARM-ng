/*
 * AppenderMap.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERMAP_HPP_FILE
#define INCLUDE_LOGGER_APPENDERMAP_HPP_FILE

#include <map>
#include <string>

#include "ConfigIO/LoggerAppenders.hpp"
#include "Logger/Appenders/Base.hpp"
#include "Logger/Exception.hpp"

// TODO: comment

namespace Logger
{

class AppenderMap
{
public:
  struct ExceptionNoSuchAppender: public Exception
  {
    ExceptionNoSuchAppender(const Location &where,
                            const char     *name):
      Exception(where, std::string("no such appender: ") +
                       ensureValidString(name) )
    {
    }
  }; // struct ExceptionNoSuchAppender

  struct ExceptionNoSuchAppenderType: public Exception
  {
    ExceptionNoSuchAppenderType(const Location &where,
                                const char     *name):
      Exception(where, std::string("no such appender type: ") +
                       ensureValidString(name) )
    {
    }
  }; // struct ExceptionNoSuchAppenderType


  explicit AppenderMap(const ConfigIO::LoggerAppenders &cfg);

  Appenders::BasePtr operator[](const std::string &name);

private:
  Appenders::BasePtr configureNew(const ConfigIO::LoggerAppenderConfig &cfg);
  void add(const std::string &name, Appenders::BasePtr ptr);

  typedef std::map<std::string, Appenders::BasePtr> AppMap;
  AppMap apps_;
}; // class AppenderMap

} // namespace Logger

#endif
