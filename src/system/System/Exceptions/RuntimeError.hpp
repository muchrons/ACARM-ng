/*
 * RuntimeError.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_RUNTIMEERROR_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_RUNTIMEERROR_HPP_FILE

/* public header */

#include "System/Exceptions/BaseSimple.hpp"

namespace System
{
namespace Exceptions
{

/** \brief base exception class for errors reported at runtime.
 */
template<typename CRTP>
class RuntimeError: public BaseSimple<RuntimeError<CRTP>, std::runtime_error>
{
private:
  typedef BaseSimple<RuntimeError<CRTP>, std::runtime_error> BaseC;

protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit RuntimeError(const T &msg):
    BaseC(msg)
  {
  }
  /** \brief create execption with given message.
   *  \param where location of exception raising.
   *  \param msg   message to represent.
   */
  template<typename T>
  RuntimeError(const typename BaseC::Location &where, const T &msg):
    BaseC(where, msg)
  {
  }
}; // class RuntimeError

} // namespace Exceptions
} // namespace System

#endif
