/*
 * BadAlloc.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_BADALLOC_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_BADALLOC_HPP_FILE

/* public header */

#include "System/Exceptions/Base.hpp"

namespace System
{
namespace Exceptions
{

/** \brief base exception class for resource allocation errors.
 */
template<typename CRTP>
class BadAlloc: public Base<BadAlloc<CRTP>, std::bad_alloc>
{
protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit BadAlloc(const T &msg):
    Exceptions::Base<BadAlloc, std::bad_alloc>(msg)
  {
  }
}; // class BadAlloc

} // namespace Exceptions
} // namespace System

#endif
