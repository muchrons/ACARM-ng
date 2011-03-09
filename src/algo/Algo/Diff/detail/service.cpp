/*
 * service.cpp
 *
 */
#include "Algo/Diff/detail/service.hpp"
#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Service &e1, const Persistency::Service &e2)
{
  if(&e1==&e2)
    return 1;

  const double name =compare( e1.getName(), e2.getName() ).get();
  const double port =compare( e1.getPort(), e2.getPort() ).get();
  const double proto=( e1.getProtocol()==e2.getProtocol() )?1:0;
  const double ref  =compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ).get();
  return (name+port+proto+ref)/4;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
