/*
 * longestCommonSubsequenceLength.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "Algo/Diff/detail/longestCommonSubsequenceLength.hpp"

using namespace std;

namespace Algo
{
namespace Diff
{
namespace detail
{

unsigned int longestCommonSubsequenceLength(const std::string &e1, const std::string &e2)
{
  //
  // http://en.wikipedia.org/wiki/Longest_common_subsequence_problem
  //

  const size_t maxR=e1.length()+1;
  const size_t maxC=e2.length()+1;

  std::vector<unsigned int> prev(maxC, 0u);
  std::vector<unsigned int> now (maxC, 0u);
  assert(prev.size()==maxC);
  assert(now.size() ==maxC);

  // fill all elements, using two vectors and swapping
  for(unsigned int r=1; r<maxR; ++r)
  {
    now.swap(prev);
    for(unsigned int c=1; c<maxC; ++c)
    {
      assert(prev[0]==0);
      assert(now [0]==0);

      if(e1[r-1]==e2[c-1])
        now[c]=prev[c-1]+1;
      else
        now[c]=std::max(now[c-1], prev[c]);
    }
  }

  assert( now.size()>0 );
  return *now.rbegin();
} // longestCommonSubsequenceLength()

} // namespace detail
} // namespace Diff
} // namespace Algo
