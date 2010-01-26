/*
 * nodeConfReader.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONFREADER_HPP_FILE
#define INCLUDE_LOGGER_NODECONFREADER_HPP_FILE

#include "Logger/NodeConf.hpp"
#include "Logger/NodeName.hpp"
#include "Logger/AppenderMap.hpp"
#include "Logger/Exception.hpp"

namespace Logger
{

/** \brief exception thrown when invalid priority is given to parsing.
 */
struct ExceptionNoSuchPriority: public Exception
{
  /** \brief create exception.
   *  \param where place where exeception has been thrown.
   *  \param name  name of appender's instance that has been required.
   */
  ExceptionNoSuchPriority(const Location &where,
                          const char     *priority):
    Exception(where, std::string("no such priority: ") +
                     ensureValidString(priority) )
  {
  }
}; // struct ExceptionNoSuchPriority


/** \brief reads node tree and gets configuration for a given node.
 *  \param nn   node name to create configration for.
 *  \param apps appenders set.
 *  \return node's configuration description.
 */
NodeConfPtr nodeConfReader(const NodeName &nn, AppenderMap &apps);

} // namespace Logger

#endif
