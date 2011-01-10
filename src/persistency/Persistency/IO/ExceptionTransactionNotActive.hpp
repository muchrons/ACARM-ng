/*
 * ExceptionTransactionNotActive.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONTRANSACTIONNOTACTIVE_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONTRANSACTIONNOTACTIVE_HPP_FILE

/* public header */

#include <string>

#include "Persistency/IO/Exception.hpp"

namespace Persistency
{
namespace IO
{

/** \brief exception thrown when transaction in use is already
 *         commited/rollbacked and cannot be used anymore.
 */
class ExceptionTransactionNotActive: public IO::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  transaction's name.
   */
  ExceptionTransactionNotActive(const Location &where, const char *name):
    IO::Exception(where, cc("transaction not active (commited/rollbacked): ", name) )
  {
  }
}; // class ExceptionTransactionNotActive

} // namespace IO
} // namespace Persistency

#endif
