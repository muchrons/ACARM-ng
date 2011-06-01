/*
 * referenceURL.cpp
 *
 */
#include <cmath>

#include "Algo/Diff/detail/referenceURL.hpp"
#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::ReferenceURL &e1, const Persistency::ReferenceURL &e2)
{
  if(&e1==&e2)
    return 1;

  Similarity s=compare( e1.getURL(),  e2.getURL()  ).get();
  s.merge( compare( e1.getName(), e2.getName() ).get() );
  return pow( s.get(), 3 );
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
