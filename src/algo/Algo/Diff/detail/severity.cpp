/*
 * severity.cpp
 *
 */
#include <cmath>

#include "Algo/Diff/detail/severity.hpp"
#include "Algo/Diff/compare.hpp"

using namespace Persistency;

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity Comparer<const Persistency::Severity>::cmp(const Persistency::Severity e1, const Persistency::Severity e2)
{
  return compare( e1.getLevel(), e2.getLevel() );
}

Similarity Comparer<const Persistency::SeverityLevel>::cmp(Persistency::SeverityLevel e1, Persistency::SeverityLevel e2)
{
  const double diff =std::abs( e1.toInt()-e2.toInt() );
  const double range=SeverityLevel::Max-SeverityLevel::Min;
  const double est  =1-(2*diff)/range;
  return Similarity::normalize(est);
}

} // namespace detail
} // namespace Diff
} // namespace Algo
