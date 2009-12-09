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

SequencedLock::SequencedLock(Mutex &m1, Mutex &m2):
  l1_( *min(&m1, &m2) ),
  l2_( *max(&m1, &m2) )
{
}

} // namespace Threads
} // namespace Base
