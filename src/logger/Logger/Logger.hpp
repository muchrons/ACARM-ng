/*
 * Logger.hpp
 *
 */
#ifndef INCLUDE_LOGGER_LOGGER_HPP_FILE
#define INCLUDE_LOGGER_LOGGER_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Logger/LoggerImpl.hpp"

// TODO: comment
// TODO: test
// TODO: implement

namespace Logger
{

#define LOGMSG_DEBUG(id, msg) LOGMSG_PRI_INTERNAL_IMPLEMENTATION((id).debug, (msg))
#define LOGMSG_INFO(id, msg)
#define LOGMSG_WARN(id, msg)
#define LOGMSG_ERROR(id, msg)
#define LOGMSG_FATAL(id, msg)

} // namespace Logger

#endif
