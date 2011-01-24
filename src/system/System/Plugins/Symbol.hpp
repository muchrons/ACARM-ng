/*
 * Symbol.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_SYMBOL_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_SYMBOL_HPP_FILE

/* public header */

#include <string>
#include <boost/type_traits/remove_pointer.hpp>
#include <cassert>

#include "System/Plugins/Handle.hpp"

namespace System
{
namespace Plugins
{

/** \brief object representation of a symbol read from handle.
 *
 *  this class wraps functionality of owning symbol and handle to library
 *  providing that, so library is loaded at least as long as the last symbol
 *  is in use.
 *
 *  \note you should NOT create this class directly. use DynamicObject instead.
 */
template<typename S>
class Symbol
{
public:
  /** \brief crete symbol asociated with handle to library it is from and name.
   *  \param h    handle to the library given symbol is from.
   *  \param name name of the symbol.
   *  \param s    symbol itself.
   */
  Symbol(HandlePtrNN h, const std::string &name, S s):
    h_(h),
    name_(name),
    s_(s)
  {
  }

  /** \brief gets name of the symbol.
   *  \return symbol name.
   */
  const std::string &name(void)
  {
    return name_;

  }
  /** \brief gets symbol itself.
   *  \return symbol.
   *  \note symbol can be NULL.
   */
  S get(void)
  {
    return s_;
  }
  /** \brief gets symbol itself - const version.
   *  \return symbol.
   *  \note symbol can be NULL.
   */
  const S get(void) const
  {
    return s_;
  }

  /** \brief gets symbol itself.
   *  \return symbol.
   *  \note symbol can be NULL.
   */
  typename boost::remove_pointer<S>::type &operator*(void)
  {
    assert(s_!=NULL);
    return *s_;
  }
  /** \brief gets symbol itself - const version.
   *  \return symbol.
   *  \note symbol can be NULL.
   */
  const typename boost::remove_pointer<S>::type &operator*(void) const
  {
    assert(s_!=NULL);
    return *s_;
  }

private:
  HandlePtrNN h_;
  std::string name_;
  S           s_;
}; // class Symbol

} // namespace Plugins
} // namespace System

#endif
