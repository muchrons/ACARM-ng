/*
 * NodeConfReader.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONFREADER_HPP_FILE
#define INCLUDE_LOGGER_NODECONFREADER_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/NodeConf.hpp"
#include "Logger/NodeName.hpp"
#include "Logger/AppenderMap.hpp"
#include "Logger/Exception.hpp"

namespace Logger
{

class NodeConfReader: private boost::noncopyable
{
public:
  /** \brief exception thrown when invalid priority is given to parsing.
   */
  struct ExceptionNoSuchPriority: public Exception
  {
    /** \brief create exception.
     *  \param where    place where exeception has been thrown.
     *  \param priority priority string that parsing failed.
     */
    ExceptionNoSuchPriority(const Location &where,
                            const char     *priority):
      Exception(where, std::string("no such priority: ") +
                       ensureValidString(priority) )
    {
    }
  }; // struct ExceptionNoSuchPriority

  /** \brief create instance.
   *  \param apps appenders map to use for getting appenders.
   */
  NodeConfReader(void);

  /** \brief parses priority string and returns enum.
   *  \param pri string ot be parsed.
   *  \return corresponding enum value.
   */
  static Priority parsePriority(const std::string &pri);

  /** \brief reads node tree and gets configuration for a given node.
   *  \param nn   node name to create configration for.
   *  \param apps appenders set.
   *  \return node's configuration description.
   */
  NodeConfPtr read(const NodeName &nn) const;

private:
  const AppenderMap appMap_;
}; // class NodeConfReader

} // namespace Logger

#endif
