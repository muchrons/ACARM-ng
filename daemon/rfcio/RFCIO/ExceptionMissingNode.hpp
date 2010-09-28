/*
 * ExceptionMissingNode.hpp
 *
 */
#ifndef INCLUDE_RFCIO_EXCEPTIONMISSINGNODE_HPP_FILE
#define INCLUDE_RFCIO_EXCEPTIONMISSINGNODE_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{

/** \brief exception thrown when required node is missing.
 */
class ExceptionMissingNode: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param parent  name of the parent node.
   *  \param missing name of the missing node.
   */
  ExceptionMissingNode(const Location &where, const std::string &parent, const std::string &missing):
    Exception(where, cc("parent node '", parent, "' is missing child '", missing, "'") )
  {
  }
}; // class ExceptionMissingNode

} // namespace RFCIO

#endif
