/*
 * NodeConf.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONF_HPP_FILE
#define INCLUDE_LOGGER_NODECONF_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "Logger/Appenders/Base.hpp"
#include "Logger/Formatter.hpp"
#include "Logger/Synchro.hpp"

namespace Logger
{

/** \brief class representing configuration of a given node.
 */
class NodeConf: private boost::noncopyable
{
public:
  /** \brief create configariotn with a given appender and default formatter.
   *  \param appender appender to be in node configuration.
   */
  explicit NodeConf(Appenders::BasePtr appender);
  /** \brief swaps contents of this node conf and one given as parameter.
   *  \param other node conf to swap content with.
   */
  void swap(NodeConf &other);
  /** \brief gives access to appender held inside.
   *  \return appender for this configuration.
   */
  Appenders::BasePtr getAppender() const
  {
    Lock lock(mutex_);
    return appender_;
  }
  /** \brief gives access to formatter.
   *  \return formatter.
   */
  Formatter getFormatter() const
  {
    Lock lock(mutex_);
    return formatter_;
  }

private:
  mutable Mutex      mutex_;
  Appenders::BasePtr appender_;
  Formatter          formatter_;    // note: formatter is held by value since
                                    //       at this moment it has just one
                                    //       possible instance and is not planned
                                    //       to expand like appenders' case.
};

/** \brief shared pointer to configuration.
 */
typedef boost::shared_ptr<NodeConf> NodeConfPtr;

} // namespace Logger

#endif
