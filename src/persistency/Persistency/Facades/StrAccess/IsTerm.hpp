/*
 * IsTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISTERM_HPP_FILE

/* public header */

#include <string>
#include <boost/type_traits/is_arithmetic.hpp>

#include "System/NoInstance.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

// by default only arithmetic types match
template<typename T>
struct IsTerm: private System::NoInstance
{
  static const bool value=boost::is_arithmetic<T>::value;
}; // struct IsTerm

// std::string
template<>
struct IsTerm<std::string>: private System::NoInstance
{
  static const bool value=true;
}; // struct IsTerm

// convert pointer->const pointer
template<typename T>
struct IsTerm<T*>: private System::NoInstance
{
  static const bool value=IsTerm<const T*>::value;
}; // struct IsTerm

// pointers are not terms
template<typename T>
struct IsTerm<const T*>: private System::NoInstance
{
  static const bool value=false;
}; // struct IsTerm

// const char * is term (exception)
template<>
struct IsTerm<const char*>: private System::NoInstance
{
  static const bool value=true;
}; // struct IsTerm

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
