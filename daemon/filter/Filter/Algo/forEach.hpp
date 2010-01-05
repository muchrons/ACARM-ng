/*
 * ForEach.hpp
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

template<typename Iter, typename FuncObj>
FuncObj forEach(Iter begin, Iter end, FuncObj f)
{
  return std::for_each(begin, end, f);
} // forEach()

} // namespace Algo
} // namespace Filter

#endif
