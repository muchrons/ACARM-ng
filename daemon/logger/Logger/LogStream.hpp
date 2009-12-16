/*
 * LogStream.hpp
 *
 */
#ifndef INCLUDE_LOGGER_LOGSTREAM_HPP_FILE
#define INCLUDE_LOGGER_LOGSTREAM_HPP_FILE

/* public header */

#include <sstream>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"

// TODO: comment
// TODO: test

namespace Logger
{

template <void (Node::*TPtr)(const char*, const char*, unsigned int, const char*) const>
class LogStream: private boost::noncopyable
{
public:
  LogStream(const Node   &log,
            const char   *file,
            const char   *call,
            unsigned int  line):
    file_(file),
    call_(call),
    line_(line),
    log_(log)
  {
  }

  ~LogStream(void)
  {
    // log message at the end of the line
    (log_.*TPtr)(file_, call_, line_, ss_.str().c_str() );
  }

  template <typename T>
  inline LogStream &operator<<(const T &t)
  {
    try
    {
      ss_<<t;
    }
    catch(...)
    {
      // this is only logging mechanism - errors here are suppressed
    }
    return *this;
  }

private:
  const char        *file_;
  const char        *call_;
  unsigned int       line_;

  const Node        &log_;
  std::stringstream  ss_;
}; // class LogStream

} // namespace Logger

#endif
