/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_RFCIO_XML_EXCEPTION_HPP_FILE
#define INCLUDE_RFCIO_XML_EXCEPTION_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{
namespace XML
{

/** \brief base for all XML-related exceptions.
 */
class Exception: public RFCIO::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    RFCIO::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace XML
} // namespace RFCIO

#endif
