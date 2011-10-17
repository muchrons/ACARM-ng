/*
 * PointerWrapper.hpp
 *
 */
#ifndef INCLUDE_ALGO_POINTERWRAPPER_HPP_FILE
#define INCLUDE_ALGO_POINTERWRAPPER_HPP_FILE

/* public header */

#include <cassert>
#include <boost/type_traits/is_pointer.hpp>

#include "System/NoInstance.hpp"
#include "Algo/MPL/EnsureRawPtr.hpp"
#include "Algo/Exception.hpp"

namespace Algo
{

namespace detail
{
template<bool isPointer, typename RawPointer>
struct PointerGetImpl: private System::NoInstance
{
  static RawPointer get(RawPointer p)
  {
    return p;
  }
}; // struct PointerGetImpl

template<typename RawPointer>
struct PointerGetImpl<false, RawPointer>: private System::NoInstance
{
  template<typename T>
  static RawPointer get(T &p)
  {
    return p.get();
  }
}; // struct PointerGetImpl
} // namespace detail


/** \brief wrapper for pointer, providing common interface and ensuring non-NULL content.
 */
template<typename PtrType>
class PointerWrapper
{
public:
  /** \brief raw pointer type. */
  typedef typename MPL::EnsureRawPtr<PtrType>::type pointer;

  explicit PointerWrapper(PtrType p):
    p_(p)
  {
    if( rawPtr()==NULL )
      throw Exception(SYSTEM_SAVE_LOCATION, "pointer cannot be NULL");
  }

  PtrType get(void) const
  {
    assert( rawPtr()!=NULL );
    return p_;
  }

  pointer operator->(void) const
  {
    assert( rawPtr()!=NULL );
    return rawPtr();
  }

private:
  pointer rawPtr(void) const
  {
    return detail::PointerGetImpl<boost::is_pointer<PtrType>::value, pointer>::get(p_);
  }

  PtrType p_;
}; // class PointerWrapper

} // namespace Algo

#endif
