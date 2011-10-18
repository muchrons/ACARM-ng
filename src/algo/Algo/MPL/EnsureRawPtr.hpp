/*
 * EnsureRawPtr.hpp
 *
 */
#ifndef INCLUDE_ALGO_MPL_ENSURERAWPTR_HPP_FILE
#define INCLUDE_ALGO_MPL_ENSURERAWPTR_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Commons/SharedPtrNotNULL.hpp"

namespace Algo
{
namespace MPL
{

namespace detail
{
/** \brief by default assume sort of smart pointer.
 */
template<typename T>
struct EnsureRawPtrImpl: private System::NoInstance
{
  typedef typename T::element_type* type;   ///< extracted element type
}; // struct EnsureRawPtrImpl

/** \brief raw pointer implementation pointer.
 */
template<typename T>
struct EnsureRawPtrImpl<T*>: private System::NoInstance
{
  typedef T* type;                          ///< just a raw pointer
}; // struct EnsureRawPtrImpl
}; // namespace detail


/** \brief helper structure for generating raw pointer type.
 *
 *  converts given (smart-)pointer to a correspongin raw pointer type, preserving CV qualifiers.
 *
 *  <code>
 *    typedef shared_ptr<double>       MyPtr;
 *    typedef shared_ptr<const double> ConstMyPtr;
 *    MyPtr ptr( new double(4.2) );
 *    // ..
 *    EnsureRawPtr<MyPtr>::type      s1(ptr); // 's1' is double*
 *    EnsureRawPtr<ConstMyPtr>::type s2(ptr); // 's2' is const double*
 *  </code>
 */
template<typename T>
struct EnsureRawPtr: private System::NoInstance
{
  /** \brief raw poitner type, from given type.
   */
  typedef typename detail::EnsureRawPtrImpl<T>::type type;
}; // struct EnsureRawPtr

} // namespace MPL
} // namespace Algo

#endif
