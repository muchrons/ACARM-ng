/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_EXCEPTION_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_EXCEPTION_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{
namespace IDMEF
{

/** \brief base for all IDMEF-related exceptions.
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

} // namespace IDMEF
} // namespace RFCIO

#endif
