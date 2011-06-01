/*
 * analyzer.cpp
 *
 */
#include "Algo/Diff/detail/analyzer.hpp"
#include "Algo/Diff/detail/all.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Analyzer &e1, const Persistency::Analyzer &e2)
{
  if(&e1==&e2)
    return 1;

  Similarity s=compare( e1.getName(), e2.getName() );
  s.merge( compare( e1.getVersion(), e2.getVersion() ) );
  s.merge( compare( e1.getOperatingSystem(), e2.getOperatingSystem() ) );
  s.merge( compare( e1.getIP(), e2.getIP() ) );
  s.merge( compare( e1.getID(), e2.getID() ) );

  return s.get();
}

} // namespace detail
} // namespace Diff
} // namespace Algo
