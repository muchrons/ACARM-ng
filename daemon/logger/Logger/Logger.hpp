/**
 * Logger.hpp
 *
 * to start logging messages you need to have Logger::Node object initialized
 * with node name. then you can use it to log messages with a given priority.
 *
 * <code>
 * Logger::Node myLogger("parent.child.subchild");
 * // ...
 * LOGMSG_INFO(myLogger, "hello logs!");
 * </code>
 *
 */
#ifndef INCLUDE_LOGGER_LOGGER_HPP_FILE
#define INCLUDE_LOGGER_LOGGER_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Logger/LoggerImpl.hpp"


namespace Logger
{

/** \brief log debug message to a given node.
 *  \param id  logger node.
 *  \param msg message to log.
 */
#define LOGMSG_DEBUG(id, msg) LOGMSG_PRI_INTERNAL_IMPLEMENTATION( (id).debug, (msg) )

/** \brief log info message to a given node.
 *  \param id  logger node.
 *  \param msg message to log.
 */
#define LOGMSG_INFO(id, msg)  LOGMSG_PRI_INTERNAL_IMPLEMENTATION( (id).info,  (msg) )

/** \brief log warn message to a given node.
 *  \param id  logger node.
 *  \param msg message to log.
 */
#define LOGMSG_WARN(id, msg)  LOGMSG_PRI_INTERNAL_IMPLEMENTATION( (id).warn,  (msg) )

/** \brief log error message to a given node.
 *  \param id  logger node.
 *  \param msg message to log.
 */
#define LOGMSG_ERROR(id, msg) LOGMSG_PRI_INTERNAL_IMPLEMENTATION( (id).error, (msg) )

/** \brief log fatal message to a given node.
 *  \param id  logger node.
 *  \param msg message to log.
 */
#define LOGMSG_FATAL(id, msg) LOGMSG_PRI_INTERNAL_IMPLEMENTATION( (id).fatal, (msg) )

} // namespace Logger

#endif
