/*
 * process.cpp
 *
 */
#include "Algo/Diff/detail/process.hpp"
#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Process &e1, const Persistency::Process &e2)
{
  if(&e1==&e2)
    return 1;

  Similarity s=compare( e1.getPath(), e2.getPath() );
  s.merge( compare( e1.getName(), e2.getName() ) );
  s.merge( compare( e1.getMD5(), e2.getMD5() ) );
  s.merge( compare( e1.getPID(), e2.getPID() ) );
  s.merge( compare( e1.getUID(), e2.getUID() ) );
  s.merge( compare( e1.getUsername(), e2.getUsername() ) );
  s.merge( compare( e1.getParameters(), e2.getParameters() ) );
  s.merge( compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ) );
  return s;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
