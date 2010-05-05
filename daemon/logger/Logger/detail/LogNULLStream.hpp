/*
 * LogNULLStream.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_LOGNULLSTREAM_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_LOGNULLSTREAM_HPP_FILE

/* public header */

#include <sstream>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "Logger/Node.hpp"


namespace Logger
{
namespace detail
{

/** \brief helper implementation that discards all that is to be logged.
 */
class LogNULLStream: private boost::noncopyable
{
public:
  /** \brief add next log part.
   *  \return self-reference to further logging.
   *  \note call never throws.
   */
  template <typename T>
  inline LogNULLStream &operator<<(const T &/*t*/)
  {
    return *this;
  }
}; // class LogNULLStream

} // namespace detail
} // namespace Logger

#endif
