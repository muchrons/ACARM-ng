/*
 * LogCallback.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_LOGCALLBACK_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_LOGCALLBACK_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Logger.hpp"

namespace Input
{
namespace Prelude
{

/** \brief class that registers logger to be used with prelude.
 */
class LogCallback: private boost::noncopyable
{
public:
  /** \brief registers prelude's callback.
   *  \param node name of node to log messages to.
   */
  LogCallback(const char *node);
  /** \brief unrsgister callback.
   *  \note after this call has been made, all logs from prelude++
   *        will be ignored.
   */
  ~LogCallback(void);

private:
  Logger::Node         log_;
  Base::Threads::Mutex mutex_;
}; // class LogCallback

} // namespace Prelude
} // namespace Input

#endif
