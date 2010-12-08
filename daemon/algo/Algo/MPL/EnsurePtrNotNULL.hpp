/*
 * EnsurePtrNotNULL.hpp
 *
 */
#ifndef INCLUDE_ALGO_MPL_ENSUREPTRNOTNULL_HPP_FILE
#define INCLUDE_ALGO_MPL_ENSUREPTRNOTNULL_HPP_FILE

/* public header */

#include "Commons/SharedPtrNotNULL.hpp"

namespace Algo
{
namespace MPL
{

/** \brief helper structure for checking if types are valid and converting to non-null smart ptr type.
 *
 *  converts given smart-pointer to some type to SharedPtrNotNULL<> of the same type,
 *  preserving CV qualifiers.
 *
 *  <code>
 *    typedef shared_ptr<double>       MyPtr;
 *    typedef shared_ptr<const double> ConstMyPtr;
 *    MyPtr ptr( new double(4.2) );
 *    // ..
 *    EnsurePtrNotNULL<MyPtr>::type      s1(ptr); // 's1' is SharedPrtNotNULL<double>
 *    EnsurePtrNotNULL<ConstMyPtr>::type s2(ptr); // 's2' is SharedPrtNotNULL<const double>
 *  </code>
 */
template<typename T>
struct EnsurePtrNotNULL
{
private:
  // short cut :)
  typedef typename T::element_type elem;

public:
  /** \brief non-null smart pointer type to a given element, respecting const and volatiles.
   */
  typedef Commons::SharedPtrNotNULL<elem> type;

private:
  // no instances needed
  EnsurePtrNotNULL(void);
}; // struct EnsurePtrNotNULL

} // namespace MPL
} // namespace Algo

#endif
