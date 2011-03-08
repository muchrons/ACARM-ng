/*
 * referenceURL.cpp
 *
 */
#include "Algo/Diff/detail/referenceURL.hpp"
#include "Algo/Diff/compare.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

namespace
{
// paramters for te linear transformation
const double a=10.0/7.0;
const double b=1.0-10.0/7.0;
} // unnamed namespace

Similarity Comparer<const Persistency::ReferenceURL>::cmp(const Persistency::ReferenceURL &e1, const Persistency::ReferenceURL &e2)
{
  if(&e1==&e2)
    return 1;

  const double url  =compare( e1.getURL(),  e2.getURL()  ).get();
  const double name =compare( e1.getName(), e2.getName() ).get();
  const double avg  =(url+name)/2;
  const double scale=a*avg+b;           // y=a*x+b
  return Similarity::normalize(scale);
}

} // namespace detail
} // namespace Diff
} // namespace Algo
