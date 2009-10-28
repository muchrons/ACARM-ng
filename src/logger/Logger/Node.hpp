/*
 * Node.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODE_HPP_FILE
#define INCLUDE_LOGGER_NODE_HPP_FILE

/* public header */

#include "Logger/NodeName.hpp"
#include "Logger/Priority.hpp"

// TODO: comment
// TODO: test
// TODO: implement

namespace Logger
{

class Node
{
public:
  Node(const NodeName &nn):
    nn_(nn)
  {
  }

  const NodeName &getName(void) const
  {
    return nn_;
  }

  void debug(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::DEBUG, file, call, line, msg);
  }

  void info( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::INFO, file, call, line, msg);
  }

  void warn( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::WARN, file, call, line, msg);
  }

  void error(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::ERROR, file, call, line, msg);
  }

  void fatal(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::FATAL, file, call, line, msg);
  }

private:
  void log(Priority      pri,
           const char   *file,
           const char   *call,
           unsigned int  line,
           const char   *msg) const;

  NodeName nn_;
}; // class Node

} // namespace Logger

#endif
