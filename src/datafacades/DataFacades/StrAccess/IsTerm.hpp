/*
 * IsTerm.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ISTERM_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ISTERM_HPP_FILE

/* public header */

#include <string>
#include <boost/asio/ip/address.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/MD5Sum.hpp"

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

/** \brief makes Persistency::MD5Sum a term.
 */
template<>
struct IsTerm<Persistency::MD5Sum>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

/** \brief makes boost::asio::ip::address a term.
 */
template<>
struct IsTerm<boost::asio::ip::address>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

/** \brief makes boost::asio::ip::address_v4 a term.
 */
template<>
struct IsTerm<boost::asio::ip::address_v4>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

/** \brief makes boost::asio::ip::address_v6 a term.
 */
template<>
struct IsTerm<boost::asio::ip::address_v6>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsTerm

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
