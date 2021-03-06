/*
 * certainty.cpp
 *
 */
#include <cmath>

#include "Algo/Diff/detail/certainty.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Certainty e1, const Persistency::Certainty e2)
{
  const double diff=std::abs( e1.get()-e2.get() );
  return Similarity::normalize(1-2*diff);
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
