/*
 * IsPointer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISPOINTER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISPOINTER_HPP_FILE

/* public header */

#include <boost/type_traits/is_pointer.hpp>

#include "System/NoInstance.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief helper that informs if a given type is a pointer type.
 */
template<typename T>
struct IsPointer: private System::NoInstance
{
  static const bool value=boost::is_pointer<T>::value;  ///< meta-program result
}; // struct IsPointer

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
