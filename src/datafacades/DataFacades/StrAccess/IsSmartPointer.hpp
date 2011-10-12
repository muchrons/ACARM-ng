/*
 * IsSmartPointer.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ISSMARTPOINTER_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ISSMARTPOINTER_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "System/NoInstance.hpp"
#include "System/SharedPtrNotNULL.hpp"
#include "Commons/SharedPtrNotNULL.hpp"

namespace Facades
{
namespace StrAccess
{

/** \brief helper indicating if a given type is a smart pointer.
 *
 *  by default element is considered as non-smart-pointer
 *
 *  this meta-program does not handle indirect types.
 */
template<typename T>
struct IsSmartPointer: private System::NoInstance
{
  static const bool value=false;    ///< meta-program result
}; // struct IsSmartPointer

/** \brief makes System::SharedPtrNotNULL<> smart poitner.
 */
template<typename T>
struct IsSmartPointer< System::SharedPtrNotNULL<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsSmartPointer

/** \brief makes Commons::SharedPtrNotNULL<> smart poitner.
 */
template<typename T>
struct IsSmartPointer< Commons::SharedPtrNotNULL<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsSmartPointer

/** \brief makes boost::shared_ptr<> smart poitner.
 */
template<typename T>
struct IsSmartPointer< boost::shared_ptr<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsSmartPointer

/** \brief makes boost::scoped_ptr<> smart poitner.
 */
template<typename T>
struct IsSmartPointer< boost::scoped_ptr<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsSmartPointer

} // namespace StrAccess
} // namespace Facades

#endif
