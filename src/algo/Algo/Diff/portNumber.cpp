/*
 * portNumber.cpp
 *
 */
#include <cmath>

#include "Algo/Diff/portNumber.hpp"

namespace Algo
{
namespace Diff
{

Similarity compare(const Persistency::PortNumber e1, const Persistency::PortNumber e2)
{
  return ( e1.get()==e2.get() )?1:0;
} // compare()

} // namespace Diff
} // namespace Algo
