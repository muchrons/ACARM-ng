/*
 * longestCommonSubsequenceLength.cpp
 *
 */
#include <algorithm>
#include <cassert>
#include <boost/numeric/ublas/matrix.hpp>

#include "Algo/Diff/detail/longestCommonSubsequenceLength.hpp"

using namespace std;
using namespace boost::numeric::ublas;

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

  matrix<unsigned int> m( e1.length()+1, e2.length()+1 );

  // fill initial elements
  for(unsigned int r=0; r<m.size1(); ++r)
    m(r,0)=0;
  for(unsigned int c=0; c<m.size2(); ++c)
    m(0,c)=0;

  // fill all other elements
  for(unsigned int r=1; r<m.size1(); ++r)
    for(unsigned int c=1; c<m.size2(); ++c)
      if(e1[r-1]==e2[c-1])
        m(r,c)=m(r-1,c-1)+1;
      else
        m(r,c)=std::max( m(r,c-1), m(r-1,c) );

  assert( m.size1()>0 );
  assert( m.size2()>0 );
  return m( m.size1()-1, m.size2()-1 );
} // longestCommonSubsequenceLength()

} // namespace detail
} // namespace Diff
} // namespace Algo
