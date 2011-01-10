/*
 * ExceptionBadAlloc.hpp
 *
 * this is implementation class of base exception for System library and
 * should not be used as a base class outside of it.
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONBADALLOC_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONBADALLOC_HPP_FILE

/* public header */

#include "System/Exceptions/BadAlloc.hpp"

namespace System
{

/** \brief internal exception class.
 */
class ExceptionBadAlloc: public Exceptions::BadAlloc
{
public:
  /** \brief create execption with given message.
   *  \param where place exception has been risen in.
   *  \param msg   message to represent.
   */
  template<typename T>
  ExceptionBadAlloc(const Location &where, const T &msg):
    Exceptions::BadAlloc(where, msg)
  {
  }
}; // class ExceptionBadAlloc

} // namespace System

#endif
