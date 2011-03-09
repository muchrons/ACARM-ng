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

  const double path  =compare( e1.getPath(), e2.getPath() ).get();
  const double name  =compare( e1.getName(), e2.getName() ).get();
  const double md5   =compare( e1.getMD5(), e2.getMD5() ).get();
  const double pid   =compare( e1.getPID(), e2.getPID() ).get();
  const double uid   =compare( e1.getUID(), e2.getUID() ).get();
  const double user  =compare( e1.getUsername(), e2.getUsername() ).get();
  const double params=compare( e1.getParameters(), e2.getParameters() ).get();
  const double url   =compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ).get();
  return (path+name+md5+pid+uid+user+params+url)/8;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
