/*
 * LoggerImpl.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_LOGGERIMPL_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_LOGGERIMPL_HPP_FILE

/* public header */

#include "Logger/detail/LogNULLStream.hpp"
#include "Logger/detail/LogStream.hpp"

namespace Logger
{
namespace detail
{

// for gcc use pretty function, otherwise just standard __func__
#ifdef __GNUC__
#  define PRETTY_FUNCTION_WRAPPER __PRETTY_FUNCTION__
#else
#  define PRETTY_FUNCTION_WRAPPER __func__
#endif

// log given message to given facility
#define LOGMSG_PRI_INTERNAL_IMPLEMENTATION(where, msg) \
  do \
  { \
    (where)(__FILE__, PRETTY_FUNCTION_WRAPPER, __LINE__, (msg)); \
  } \
  while(false)

// log message in stream-like manier
#define LOGMSG_PRI_INTERNAL_STREAM_IMPLEMENTATION(id, method) \
  ::Logger::detail::LogStream< &::Logger::Node::method >( (id), __FILE__, PRETTY_FUNCTION_WRAPPER, __LINE__ )


// debug logs are to be compiled-out from code
#ifndef NDEBUG

// log given message to given facility - debug macro
#define LOGMSG_PRI_INTERNAL_IMPLEMENTATION_DEBUG(where, msg) \
            LOGMSG_PRI_INTERNAL_IMPLEMENTATION(where, msg)

// log message in stream-like manier - debug macro
#define LOGMSG_PRI_INTERNAL_STREAM_IMPLEMENTATION_DEBUG(id, method) \
            LOGMSG_PRI_INTERNAL_STREAM_IMPLEMENTATION(id, method)

#else  // else: NDEBUG

// log given message to given facility - debug macro
#define LOGMSG_PRI_INTERNAL_IMPLEMENTATION_DEBUG(where, msg) \
            do { } while(false)

// log message in stream-like manier - debug macro
#define LOGMSG_PRI_INTERNAL_STREAM_IMPLEMENTATION_DEBUG(id, method) \
            while(false) ::Logger::detail::LogNULLStream()

#endif // NDEBUG

} // namespace detail
} // namespace Logger

#endif
