/*
 * LoggerImpl.hpp
 *
 */
#ifndef INCLUDE_LOGGER_LOGGERIMPL_HPP_FILE
#define INCLUDE_LOGGER_LOGGERIMPL_HPP_FILE

/* public header */

// TODO: test

namespace Logger
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
  while(0)

} // namespace Logger

#endif
