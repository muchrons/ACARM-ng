/*
 * Base.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_BASE_HPP_FILE

#include <boost/shared_ptr.hpp>

#include "Logger/Synchro.hpp"

// TODO: comment

namespace Logger
{
namespace Appenders
{

class Base
{
public:
  void append(const std::string &str)
  {
    Lock lock(mutex_);
    appendImpl(str);
  }

private:
  virtual void appendImpl(const std::string &str) = 0;

  Mutex mutex_;
}; // class Base


typedef boost::shared_ptr<Base> BasePtr;

} // namespace Appenders
} // namespace Logger

#endif
