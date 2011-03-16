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

  Similarity s=compare( e1.getName(), e2.getName() );
  s.merge( compare( e1.getPort(), e2.getPort() ) );
  s.merge( ( e1.getProtocol()==e2.getProtocol() )?1:0 );
  s.merge( compare( e1.getReferenceURL().get(), e2.getReferenceURL().get() ) );
  return s;
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
