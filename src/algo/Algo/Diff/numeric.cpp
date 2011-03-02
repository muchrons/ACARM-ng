/*
 * numeric.cpp
 *
 */
#include "Algo/Diff/numeric.hpp"

namespace Algo
{
namespace Diff
{

namespace
{
template<typename T>
Similarity numericCompareImpl(const T t1, const T t2)
{
  return (t1==t2)?1:0;
} // numericCompareImpl()
} // unnamed namespace


Similarity compare(unsigned long e1, unsigned long e2)
{
  return numericCompareImpl(e1, e2);
} // compare()

Similarity compare(long e1, long e2)
{
  return numericCompareImpl(e1, e2);
} // compare()

Similarity compare(pid_t e1, pid_t e2)
{
  return numericCompareImpl(e1, e2);
} // compare()

} // namespace Diff
} // namespace Algo
