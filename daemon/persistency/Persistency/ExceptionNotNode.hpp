/*
 * ExceptionNotNode.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONNOTNODE_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONNOTNODE_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorw when node-only operation has been requested on leaf.
 */
class ExceptionNotNode: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param name  name of the node (leaf).
   */
  ExceptionNotNode(const Location &where, const char *name):
    Exception(where, std::string("element is not node: ") + ensureString(name) )
  {
  }
}; // class ExceptionNotNode

} // namespace Persistency

#endif
