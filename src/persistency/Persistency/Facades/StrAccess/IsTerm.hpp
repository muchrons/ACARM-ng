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

/** \brief defines what types are terms.
 *
 *  by default only arithmetic types match.
 *
 *  indirection is not handeled by this meta-program.
 */
template<typename T>
struct IsTerm: private System::NoInstance
{
  static const bool value=boost::is_arithmetic<T>::value;   ///< meta-program result
}; // struct IsTerm

/** \brief makes std::string a term.
 */
template<>
struct IsTerm<std::string>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

/** \brief threads pointers the same way as const pointers.
 */
template<typename T>
struct IsTerm<T*>: private System::NoInstance
{
  static const bool value=IsTerm<const T*>::value;      ///< meta-program result
}; // struct IsTerm

/** \brief random pointers are not terms.
 */
template<typename T>
struct IsTerm<const T*>: private System::NoInstance
{
  static const bool value=false;    ///< meta-program result
}; // struct IsTerm

/** \brief const char * is a term (exception).
 */
template<>
struct IsTerm<const char*>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

/** \brief char[] is a term (exception).
 */
template<unsigned int N>
struct IsTerm<char[N]>: private System::NoInstance
{
  static const bool value=IsTerm<char*>::value;         ///< meta-program result
}; // struct IsTerm

/** \brief const char[] is a term (exception).
 */
template<unsigned int N>
struct IsTerm<const char[N]>: private System::NoInstance
{
  static const bool value=IsTerm<const char*>::value;   ///< meta-program result
}; // struct IsTerm

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
