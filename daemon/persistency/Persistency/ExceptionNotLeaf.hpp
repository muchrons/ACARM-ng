/*
 * ExceptionNotLeaf.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONNOTLEAF_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONNOTLEAF_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorw when leaf-only operation has been requested
 *         on non-leaf node.
 */
class ExceptionNotLeaf: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param name  name of the node (leaf).
   */
  ExceptionNotLeaf(const Location &where, const char *name):
    Exception(where, std::string("element is not leaf: ") + ensureString(name) )
  {
  }
}; // class ExceptionNotNode

} // namespace Persistency

#endif
