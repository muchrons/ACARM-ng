/*
 * LogFromHeader.t.hpp
 *
 */
#ifndef INCLUDE_LOGGER_LOGFROMHEADER_T_HPP_FILE
#define INCLUDE_LOGGER_LOGFROMHEADER_T_HPP_FILE

#include "Logger/Logger.hpp"

namespace Logger
{

inline void doSomeLogging(void)
{
  Node log("logger.dosomelogging");
  LOGMSG_DEBUG(log, "hello header world");
}

} // namespace Logger

#endif
