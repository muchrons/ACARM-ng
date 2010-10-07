/*
 * ExceptionInvalidElement.hpp
 *
 */
#ifndef INCLUDE_RFCIO_EXCEPTIONINVALIDELEMENT_HPP_FILE
#define INCLUDE_RFCIO_EXCEPTIONINVALIDELEMENT_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{

/** \brief exception thrown when required element exist, but is invalid (wrong type/value).
 */
class ExceptionInvalidElement: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param node    node that is invalid.
   *  \param comment wht element's value is invalid.
   */
  ExceptionInvalidElement(const Location &where, const std::string &node, const std::string &comment):
    Exception(where, cc("parent node '", node, "' is invalid: ", comment) )
  {
  }
}; // class ExceptionInvalidElement

} // namespace RFCIO

#endif
