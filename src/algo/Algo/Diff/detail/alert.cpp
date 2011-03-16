/*
 * alert.cpp
 *
 */
#include <cmath>

#include "Algo/Diff/detail/alert.hpp"
#include "Algo/Diff/detail/all.hpp"

using namespace Persistency;

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity compare(const Persistency::Alert &e1, const Persistency::Alert &e2)
{
  Similarity s=compare( e1.getName(), e2.getName() );
  s.merge( compare( e1.getAnalyzers(), e2.getAnalyzers() ) );
  s.merge( compare( e1.getDetectionTime(), e2.getDetectionTime() ) );
  s.merge( compare( e1.getCreationTime(), e2.getCreationTime() ) );
  s.merge( compare( e1.getSeverity(), e2.getSeverity() ) );
  s.merge( compare( e1.getCertainty(), e2.getCertainty() ) );
  s.merge( compare( e1.getDescription(), e2.getDescription() ) );
  s.merge( compare( e1.getSourceHosts(), e2.getSourceHosts() ) );
  s.merge( compare( e1.getTargetHosts(), e2.getTargetHosts() ) );
  return s.get();
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
