/*
 * host.cpp
 *
 */
#include "Algo/Diff/detail/host.hpp"
#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Host &e1, const Persistency::Host &e2)
{
  if(&e1==&e2)
    return 1;

  Similarity s=compare( e1.getIP(), e2.getIP() );
  s.merge( compare( e1.getNetmask(), e2.getNetmask() ) );
  s.merge( compare( e1.getOperatingSystem(), e2.getOperatingSystem() ) );
  s.merge( compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ) );
  s.merge( compare( e1.getServices(), e2.getServices() ) );
  s.merge( compare( e1.getProcesses(), e2.getProcesses() ) );
  s.merge( compare( e1.getName(), e2.getName() ) );
  return s;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
