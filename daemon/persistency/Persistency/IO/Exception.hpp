/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_EXCEPTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_EXCEPTION_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"
#include "Persistency/IO/Exception.hpp"

namespace Persistency
{
namespace IO
{

/** \brief base for all IO-related exceptions.
 */
class Exception: public Persistency::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Persistency::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace IO
} // namespace Persistency

#endif
