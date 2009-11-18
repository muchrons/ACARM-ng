/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_EXCEPTION_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_EXCEPTION_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{
namespace Stubs
{

/** \brief base for all stubs-related exceptions (derived from Persistency).
 */
class Exception: public Persistency::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  Exception(const char *where, const char *msg):
    Persistency::Exception(where, ensureString(msg) )
  {
  }
}; // class Exception

} // namespace Stubs
} // namespace Persistency

#endif
