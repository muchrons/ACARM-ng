/*
 * IsSmartPointer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISSMARTPOINTER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISSMARTPOINTER_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "System/NoInstance.hpp"
#include "System/SharedPtrNotNULL.hpp"
#include "Commons/SharedPtrNotNULL.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

template<typename T>
struct IsSmartPointer: private System::NoInstance
{
  static const bool value=false;
}; // struct IsSmartPointer

template<typename T>
struct IsSmartPointer< System::SharedPtrNotNULL<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsSmartPointer

template<typename T>
struct IsSmartPointer< Commons::SharedPtrNotNULL<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsSmartPointer

template<typename T>
struct IsSmartPointer< boost::shared_ptr<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsSmartPointer

template<typename T>
struct IsSmartPointer< boost::scoped_ptr<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsSmartPointer

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
