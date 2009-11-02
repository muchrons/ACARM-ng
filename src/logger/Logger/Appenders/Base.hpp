/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE

#include <boost/shared_ptr.hpp>

#include "Logger/Synchro.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief common interface for all appenders.
 */
class Base
{
public:
  /** \brief appends given string (calls appender) implenetation.
   *  \param str message to be appended.
   *  \note call is thread-safe.
   */
  void append(const std::string &str)
  {
    Lock lock(mutex_);
    appendImpl(str);
  }

private:
  /** \brief template-method pattern - user implements this call.
   *  \param str message to be appended.
   */
  virtual void appendImpl(const std::string &str) = 0;

  Mutex mutex_;
}; // class Base


/** \brief shared pointer for base class.
 */
typedef boost::shared_ptr<Base> BasePtr;

} // namespace Appenders
} // namespace Logger

#endif
