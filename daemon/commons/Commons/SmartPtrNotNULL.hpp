/*
 * SmartPtrNotNull.hpp
 *
 */
#ifndef INCLUDE_COMMONS_SMARTPTRNOTNULL_HPP_FILE
#define INCLUDE_COMMONS_SMARTPTRNOTNULL_HPP_FILE

/* public header */

#include "Commons/ExceptionUnexpectedNULL.hpp"

namespace Commons
{

template<typename TSmartPtr>
struct SmartPtrNotNULL: public TSmartPtr
{
  template<typename T>
  SmartPtrNotNULL(T t):
    TSmartPtr(t)
  {
    if( TSmartPtr::get()==NULL )
      throw ExceptionUnexpectedNULL(__FILE__);
  }
}; // struct SMartPtrNotNULL

} // namespace Commons

#endif
