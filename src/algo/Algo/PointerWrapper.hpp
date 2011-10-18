/*
 * PointerWrapper.hpp
 *
 */
#ifndef INCLUDE_ALGO_POINTERWRAPPER_HPP_FILE
#define INCLUDE_ALGO_POINTERWRAPPER_HPP_FILE

/* public header */

#include <cassert>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/static_assert.hpp>

#include "System/NoInstance.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/MPL/EnsureRawPtr.hpp"
#include "Algo/Exception.hpp"

namespace Algo
{

namespace detail
{
/** \brief helper object returning pointer from pointer (i.e. forward).
 */
template<bool isPointer, typename RawPointer>
struct PointerGetImpl: private System::NoInstance
{
  /** \brief forwards argument, for interface comatibility.
   */
  static RawPointer get(RawPointer p)
  {
    return p;
  }
}; // struct PointerGetImpl

/** \brief helper object returning raw pointer from smart pointer.
 */
template<typename RawPointer>
struct PointerGetImpl<false, RawPointer>: private System::NoInstance
{
  /** \brief gets raw pointer from given smart pointer.
   *  \param p smart pointer to extrac raw pointer from.
   *  \return extracted raw pointer.
   */
  template<typename T>
  static RawPointer get(T &p)
  {
    return p.get();
  }
}; // struct PointerGetImpl


/** \brief basic (autoamtic) conversion.
 */
template<typename To, typename From>
struct PointerConverter
{
  /** \brief generic implementation performing automatic conversions.
   *  \param in source type to convert from.
   *  \return destination type to convert to.
   */
  static To convert(From in)
  {
    // NOTE: this blocks problematic conversion, when trying to create smart pointer from raw pointer (double delete).
    BOOST_STATIC_ASSERT( boost::is_pointer<To>::value || !boost::is_pointer<From>::value );
    return To(in);
  }
}; // struct PointerConverter

/** \brief SharedPtrNotNULL<> -> shared_ptr<> converter
 */
template<typename T, typename U>
struct PointerConverter< boost::shared_ptr<T>, Commons::SharedPtrNotNULL<U> >
{
  /** \brief convert smart pointer types.
   *  \param in pointer to conver from.
   *  \return destination pointer type.
   */
  static boost::shared_ptr<T> convert(Commons::SharedPtrNotNULL<U> in)
  {
    return in.shared_ptr();
  }
}; // struct PointerConverter

/** \brief SharedPtrNotNULL<> -> ptr converter
 */
template<typename T, typename U>
struct PointerConverter<T*, Commons::SharedPtrNotNULL<U> >
{
  /** \brief converts smart pointer to raw pointer type.
   *  \param in source to convert from.
   *  \return raw pointer.
   */
  static T* convert(Commons::SharedPtrNotNULL<U> in)
  {
    return in.get();
  }
}; // struct PointerConverter

/** \brief shared_ptr<> -> ptr converter
 */
template<typename T, typename U>
struct PointerConverter<T*, boost::shared_ptr<U> >
{
  /** \brief converts smart pointer to raw pointer type.
   *  \param in source to convert from.
   *  \return raw pointer.
   */
  static T* convert(boost::shared_ptr<U> in)
  {
    return in.get();
  }
}; // struct PointerConverter
} // namespace detail


/** \brief wrapper for pointer, providing common interface and ensuring non-NULL content.
 */
template<typename PtrType>
class PointerWrapper
{
public:
  /** \brief exact element type, given by the user. */
  typedef PtrType                                                                  element_type;
  /** \brief exact element type, given by the user. */
  typedef element_type                                                             value_type;
  /** \brief raw pointer type. */
  typedef typename MPL::EnsureRawPtr<PtrType>::type                                pointer;
  /** \brief smart pointer to data */
  typedef Commons::SharedPtrNotNULL<typename boost::remove_pointer<pointer>::type> smart_pointer;

  /** \brief create object from corresponding type.
   *  \param p corresponding type to create object from.
   */
  template<typename T>
  PointerWrapper(const PointerWrapper<T> &p):
    p_( detail::PointerConverter<PtrType, T>::convert(p.get()) )
  {
  }
  /** \brief create object from any, possible type.
   *  \param p element to create from.
   */
  template<typename T>
  explicit PointerWrapper(T p):
    p_( detail::PointerConverter<PtrType, T>::convert(p) )
  {
    if( rawPtr()==NULL )
      throw Exception(SYSTEM_SAVE_LOCATION, "pointer cannot be NULL");
  }

  /** \brief gets pointer, as held inside the object.
   *  \return pointer is a user-requested form.
   */
  PtrType get(void) const
  {
    assert( rawPtr()!=NULL );
    return p_;
  }
  /** \brief arrow operator to get access to pointer's data.
   *  \return raw pointer type.
   */
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
