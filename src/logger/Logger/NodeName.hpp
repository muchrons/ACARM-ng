/*
 * NodeName.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODENAME_HPP_FILE
#define INCLUDE_LOGGER_NODENAME_HPP_FILE

/* public header */

#include <string>

#include "Logger/Exception.hpp"
#include "Logger/ExceptionNullParameter.hpp"

namespace Logger
{

/** \brief exception thrown when given node name is not valid.
 */
struct ExceptionInvalidNodeName: public Exception
{
  /** \brief create execption with given name as a parameter.
   *  \param nodeName invlaid node name that has been given.
   */
  explicit ExceptionInvalidNodeName(const char *nodeName):
    Exception( std::string("invalid node name: ") + nodeName )
  {
  }
}; // struct ExceptionInvalidNodeName


// TODO: comment
// TODO: test

class NodeName
{
public:
  NodeName(const char *name);
  NodeName(const NodeName &parentNode, const char *surfix);

  const std::string get() const
  {
    return name_;
  }

private:
  const char *validate(const char *name);

  std::string name_;
}; // class NodeName

} // namespace Logger

#endif
