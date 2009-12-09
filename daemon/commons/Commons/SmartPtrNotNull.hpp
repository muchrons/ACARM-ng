/*
 * SmartPtrNotNull.hpp
 *
 */
#ifndef INCLUDE_BASE_SMARTPTRNOTNULL_HPP_FILE
#define INCLUDE_BASE_SMARTPTRNOTNULL_HPP_FILE

/* public header */

namespace Base
{

template<typename TSmartPtr>
struct SmartPtrNotNull: public TSmartPtr
{
  template<typename T>
  SmartPtrNotNull(T &t):
    TSmartPtr(t)
  {
  }
}; // struct SMartPtrNotNull

} // namespace Base

#endif
