/*
 * EnsureNodePtrNotNULL.hpp
 *
 */
#ifndef INCLUDE_ALGO_MPL_ENSURENODEPTRNOTNULL_HPP_FILE
#define INCLUDE_ALGO_MPL_ENSURENODEPTRNOTNULL_HPP_FILE

/* public header */

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/GraphNode.hpp"
#include "Algo/MPL/EnsurePtrNotNULL.hpp"

namespace Algo
{
namespace MPL
{

/** \brief helper structure for converting GraphNode to proepr type
 *
 *  converts given smart-pointer to some type to SharedPtrNotNULL<> of the same type,
 *  preserving CV qualifiers. param type is assumed to be GraphNode.
 *
 *  <code>
 *    typedef shared_ptr<GraphNode>       GraphNodePtr;
 *    typedef shared_ptr<const GraphNode> ConstGraphNodePtr;
 *    GraphNodePtr ptr(...);
 *    // ..
 *    EnsureNodePtrNotNULL<GraphNodePtr>::type      s1(ptr); // 's1' is SharedPrtNotNULL<GraphNode>
 *    EnsureNodePtrNotNULL<ConstGraphNodePtr>::type s2(ptr); // 's2' is SharedPrtNotNULL<const GraphNode>
 *  </code>
 */
template<typename T>
struct EnsureNodePtrNotNULL: private System::NoInstance
{
private:
  // short cut :)
  typedef typename T::element_type elem;

  // check if type is correct - helpers
  typedef typename boost::remove_const<elem>::type                     elem_non_const;
  typedef Persistency::GraphNode                                       elem_expected;
  typedef typename boost::is_same<elem_non_const, elem_expected>::type Checker;
  BOOST_STATIC_ASSERT( Checker::value );

public:
  /** \brief non-null smart pointer type to a given element, respecting const and volatiles.
   */
  typedef typename EnsurePtrNotNULL<T>::type type;
}; // struct EnsureNodePtrNotNULL

} // namespace MPL
} // namespace Algo

#endif
