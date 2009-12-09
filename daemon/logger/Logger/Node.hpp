/*
 * Node.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODE_HPP_FILE
#define INCLUDE_LOGGER_NODE_HPP_FILE

/* public header */

#include "Logger/NodeName.hpp"
#include "Logger/Priority.hpp"
#include "Logger/NodeConfPtr.hpp"

// TODO: add '...' interface for logging

namespace Logger
{

/** \brief repreesnts node of a logger in a loggers' tree.
 *  \note this class is NOT thread safe, although logging IS thread safe.
 *        in practice this means that as long as single Node instance is
 *        not shared ammong threads everything will work fine.
 */
class Node
{
public:
  /** \brief creates node of a given node name.
   *  \param nn name of a node to create.
   */
  explicit Node(const NodeName &nn);

  /** \brief deallocates object.
   */
  ~Node(void);

  /** \brief gets name of this node.
   *  \return name of this node.
   */
  const NodeName &getName(void) const
  {
    return nn_;
  }

  /** \brief logs debug message.
   *  \param file file name where messages is logged.
   *  \param call function/method where log was called.
   *  \param line line number in a file where logger was called.
   *  \param msg  message to be logged.
   */
  void debug(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::DEBUG, file, call, line, msg);
  }

  /** \brief logs info message.
   *  \param file file name where messages is logged.
   *  \param call function/method where log was called.
   *  \param line line number in a file where logger was called.
   *  \param msg  message to be logged.
   */
  void info( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::INFO, file, call, line, msg);
  }

  /** \brief logs warning message.
   *  \param file file name where messages is logged.
   *  \param call function/method where log was called.
   *  \param line line number in a file where logger was called.
   *  \param msg  message to be logged.
   */
  void warn( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::WARN, file, call, line, msg);
  }

  /** \brief logs error message.
   *  \param file file name where messages is logged.
   *  \param call function/method where log was called.
   *  \param line line number in a file where logger was called.
   *  \param msg  message to be logged.
   */
  void error(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    log(Priority::ERROR, file, call, line, msg);
  }

  /** \brief logs fatal message.
   *  \param file file name where messages is logged.
   *  \param call function/method where log was called.
   *  \param line line number in a file where logger was called.
   *  \param msg  message to be logged.
   */
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

  NodeName    nn_;
  NodeConfPtr nc_;
}; // class Node

} // namespace Logger

#endif
