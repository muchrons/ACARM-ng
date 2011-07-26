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

// std::string is term
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

// const char * is a term (exception)
template<>
struct IsTerm<const char*>: private System::NoInstance
{
  static const bool value=true;
}; // struct IsTerm

// char[] is a term (exception)
template<unsigned int N>
struct IsTerm<char[N]>: private System::NoInstance
{
  static const bool value=IsTerm<char*>::value;
}; // struct IsTerm

// const char[] is a term (exception)
template<unsigned int N>
struct IsTerm<const char[N]>: private System::NoInstance
{
  static const bool value=IsTerm<const char*>::value;
}; // struct IsTerm

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
