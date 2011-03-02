/*
 * host.cpp
 *
 */
#include "Algo/Diff/host.hpp"
#include "Algo/Diff/all.hpp"

namespace Algo
{
namespace Diff
{

Similarity compare(const Persistency::Host &e1, const Persistency::Host &e2)
{
  if(&e1==&e2)
    return 1;

  const double ip   =compare( e1.getIP(), e2.getIP() ).get();
  const double mask =compare( e1.getNetmask(), e2.getNetmask() ).get();
  const double os   =compare( e1.getOperatingSystem(), e2.getOperatingSystem() ).get();
  const double url  =compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ).get();
  const double srvs =compare( e1.getServices(), e2.getServices() ).get();
  const double procs=compare( e1.getProcesses(), e2.getProcesses() ).get();
  const double name =compare( e1.getName(), e2.getName() ).get();
  return (ip+mask+os+url+srvs+procs+name)/7;
} // compare()

} // namespace Diff
} // namespace Algo
