/*
 * string.cpp
 *
 */
#include <algorithm>
#include <vector>
#include <cassert>

#include "Algo/Diff/detail/string.hpp"
#include "Algo/Diff/detail/longestCommonSubsequenceLength.hpp"

using namespace std;

namespace Algo
{
namespace Diff
{
namespace detail
{

Similarity Comparer<const std::string>::cmp(const std::string &e1, const std::string &e2)
{
  if(&e1==&e2)
    return 1;

  // ensure signle order, that ensurtes best performance
  if( e1.length()<e2.length() )
    return cmp(e2, e1);
  assert( e1.length()>=e2.length() );

  // check for the special cases (speed up)
  if( e1.length()==0 )
  {
    if( e2.length()==0 )
      return 1;
    else
      return 0;
  }
  if(e1==e2)
    return 1;

  // perform full computation
  const unsigned int lcsLen=longestCommonSubsequenceLength(e1, e2);
  return Similarity( (2.0*lcsLen)/(e1.length()+e2.length()) );
}

} // namespace detail
} // namespace Diff
} // namespace Algo
