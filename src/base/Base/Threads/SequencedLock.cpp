/*
 * SequencedLock.cpp
 *
 */
#include <algorithm>

#include "Base/Threads/SequencedLock.hpp"

using namespace std;

namespace Base
{
namespace Threads
{

namespace
{
Mutex *testedMin(Mutex *m1, Mutex *m2)
{
  assert(m1!=m2 && "the same mutexe passed twice");
  return min(m1, m2);
}
} // unnamed namespace

SequencedLock::SequencedLock(Mutex &m1, Mutex &m2):
  l1_( *testedMin(&m1, &m2) ),
  l2_( *max      (&m1, &m2) )
{
}

} // namespace Threads
} // namespace Base
