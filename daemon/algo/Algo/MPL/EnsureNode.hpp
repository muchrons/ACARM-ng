/*
 * EnsureNode.hpp
 *
 */
#ifndef INCLUDE_ALGO_MPL_ENSURENODE_HPP_FILE
#define INCLUDE_ALGO_MPL_ENSURENODE_HPP_FILE

/* public header */

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNode.hpp"

namespace Algo
{
namespace MPL
{

/** \brief helper structure for checking if types are valid and converting to peroper form.
 */
template<typename T>
struct EnsureNode
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
  typedef Commons::SharedPtrNotNULL<elem> type;

private:
  // no instances needed
  EnsureNode(void);
}; // struct EnsureNode

} // namespace MPL
} // namespace Algo

#endif
