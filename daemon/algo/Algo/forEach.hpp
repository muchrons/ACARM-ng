/*
 * forEach.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACH_HPP_FILE
#define INCLUDE_ALGO_FOREACH_HPP_FILE

/* public header */

#include <algorithm>

namespace Algo
{
/** \brief traverses all elements in a given range.
 *
 *  this algorithm is here onyl for keeping all algorithms in one place.
 *
 *  \param begin first iterator to check.
 *  \param end   end-indicating iterator.
 *  \param f     function object to apply to each element.
 *  \return copy of funciton object, that traversed through all elements.
 *  \note algorithm does not go deeper into tree.
 */
template<typename Iter, typename FuncObj>
FuncObj forEach(Iter begin, Iter end, FuncObj f)
{
  return std::for_each(begin, end, f);
} // forEach()

} // namespace Algo

#endif
