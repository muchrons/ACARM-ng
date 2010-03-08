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

namespace Logger
{
/** \brief map that creates required appenders based on their configuration.
 */
class AppenderMap
{
public:
  /** \brief exception thrown when requested appender is not registered.
   */
  struct ExceptionNoSuchAppender: public Exception
  {
    /** \brief create exception.
     *  \param where place where exeception has been thrown.
     *  \param name  name of appender's instance that has been required.
     */
    ExceptionNoSuchAppender(const Location &where,
                            const char     *name):
      Exception(where, std::string("no such appender: ") +
                       ensureValidString(name) )
    {
    }
  }; // struct ExceptionNoSuchAppender

  /** \brief exception throw when configuration requires non-exisitng type
   *         to be created.
   */
  struct ExceptionNoSuchAppenderType: public Exception
  {
    /** \brief create exception.
     *  \param where place where exeception has been thrown.
     *  \param name  name of appender's type that has been required.
     */
    ExceptionNoSuchAppenderType(const Location &where,
                                const char     *name):
      Exception(where, std::string("no such appender type: ") +
                       ensureValidString(name) )
    {
    }
  }; // struct ExceptionNoSuchAppenderType


  /** \brief create appender's map from given configuration.
   *  \param cfg configuration to read appender's config from.
   */
  explicit AppenderMap(const ConfigIO::LoggerAppenders &cfg);
  /** \brief get appender instance of a given name.
   *  \param name name of instance to get.
   *  \return base pointer to proper instance.
   *  \note if instance gy a given anme does not exist, call throws.
   */
  Appenders::BasePtr operator[](const std::string &name) const;

private:
  Appenders::BasePtr configureNew(const ConfigIO::LoggerAppenderConfig &cfg);
  void add(const std::string &name, Appenders::BasePtr ptr);

  typedef std::map<std::string, Appenders::BasePtr> AppMap;
  AppMap apps_;
}; // class AppenderMap

} // namespace Logger

#endif
