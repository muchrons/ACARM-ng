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
   *  \param where    place where exception has been raised.
   *  \param nodeName invlaid node name that has been given.
   */
  explicit ExceptionInvalidNodeName(const Location &where, const char *nodeName):
    Exception(where, std::string("invalid node name: ") +
                     ensureValidString(nodeName) )
  {
  }
}; // struct ExceptionInvalidNodeName


/** \brief repreesnts name of a node.
 */
class NodeName
{
public:
  /** \brief creates node name from a given stirng.
   *  \param name name to create node name from.
   *  \note notice that this c-tor is not explicit to make it behave more
   *        intuitive when passing as a call parameter.
   */
  NodeName(const char *name);
  /** \brief creates child node for a given parent.
   *  \param parentNode parent node for this one.
   *  \param surfix     name surfix to be used.
   */
  NodeName(const NodeName &parentNode, const char *surfix);

  /** \brief returns node name.
   *  \return node name.
   */
  const std::string &get() const
  {
    return name_;
  }

private:
  const char *validate(const char *name);

  std::string name_;
}; // class NodeName

} // namespace Logger

#endif
