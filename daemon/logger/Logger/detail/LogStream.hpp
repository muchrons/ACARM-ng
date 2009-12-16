/*
 * LogStream.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_LOGSTREAM_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_LOGSTREAM_HPP_FILE

/* public header */

#include <sstream>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "Logger/Node.hpp"


namespace Logger
{
namespace detail
{
/** \brief helper object that creates log message in stream-like form.
 *  \param TPtr pointer to Node's logging method, for a given priority.
 */
template <void (Node::*TPtr)(const char*, const char*, unsigned int, const char*) const>
class LogStream: private boost::noncopyable
{
public:
  /** \brief starts log line.
   *  \param log  node to log to.
   *  \param file file name when log has been called.
   *  \param call funciton name.
   *  \param line line number log has been generated in.
   */
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

  /** \brief log message with logger.
   *  \note if nothing has been added, no log is generated.
   */
  ~LogStream(void)
  {
    // log message at the end of the line, but only if something
    // really happened (i.e. something was added to stream).
    const std::string &str=ss_.str();
    const char        *msg=str.c_str();
    assert(msg!=NULL);
    if(msg[0]!=0)   // i.e. not empty
      (log_.*TPtr)(file_, call_, line_, msg);
  }

  /** \brief add next log part.
   *  \param t next message part.
   *  \return self-reference to further logging.
   *  \note call never throws.
   */
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

} // namespace detail
} // namespace Logger

#endif
