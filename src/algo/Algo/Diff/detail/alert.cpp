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
  /*            
  const double name =compare( e1.getName(), e2.getName() ).get();
  const double anlzs=compare( e1.getAnalyzers(), e2.getAnalyzers() ).get();
  const double det  =compare( e1.getDetectionTime(), e2.getDetectionTime() ).get();
  const double creat=compare( e1.getCreationTime(), e2.getCreationTime() ).get();
  const double sev  =compare( e1.getSeverity(), e2.getSeverity() ).get();
  const double cert =compare( e1.getCertainty(), e2.getCertainty() ).get();
  const double desc =compare( e1.getDescription(), e2.getDescription() ).get();
  const double srcs =compare( e1.getSourceHosts(), e2.getSourceHosts() ).get();
  const double tgts =compare( e1.getTargetHosts(), e2.getTargetHosts() ).get();
  return (name+anlzs+det+creat+sev+cert+desc+srcs+tgts)/9;
  */                
return 0;                   
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
