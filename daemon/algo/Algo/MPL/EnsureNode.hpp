/*
 * EnsureNode.hpp
 *
 */
#ifndef INCLUDE_ALGO_MPL_ENSURENODE_HPP_FILE
#define INCLUDE_ALGO_MPL_ENSURENODE_HPP_FILE

/* public header */

#include <boost/type_traits/remove_const.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/equal.hpp>

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

public:
  /** \brief non-null smart pointer type to a given element, respecting const and volatiles.
   */
  typedef Commons::SharedPtrNotNULL<elem> type;

private:
  // no instances needed
  EnsureNode(void);

  // check if type is correct
  typedef typename boost::remove_const<T>::type elem_non_const;
  typedef Persistency::GraphNode                elem_expected;
  BOOST_STATIC_ASSERT( boost::mpl::equal<elem_non_const, elem_expected>::type::value );
}; // struct EnsureNode

} // namespace MPL
} // namespace Algo

#endif
