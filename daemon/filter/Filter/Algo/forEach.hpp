/*
 * forEach.hpp
 *
 */
#ifndef INCLUDE_FILTER_ALGO_FOREACH_HPP_FILE
#define INCLUDE_FILTER_ALGO_FOREACH_HPP_FILE

/* public header */

#include <algorithm>

namespace Filter
{
namespace Algo
{
/** \brief traverses all elements in a given range.
 *  \param begin first iterator to check.
 *  \param end   end-indicating iterator.
 *  \param f     function object to apply to each element.
 *  \note algorithm does not go deeper into tree.
 */
template<typename Iter, typename FuncObj>
FuncObj forEach(Iter begin, Iter end, FuncObj f)
{
  return std::for_each(begin, end, f);
} // forEach()

} // namespace Algo
} // namespace Filter

#endif
