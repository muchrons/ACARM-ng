/*
 * string.cpp
 *
 */
#include <algorithm>
#include <cassert>
#include <boost/numeric/ublas/matrix.hpp>

#include "Algo/Diff/string.hpp"

using namespace boost::numeric::ublas;


namespace Algo
{
namespace Diff
{

namespace
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
} // unnamed namespace

Similarity compare(const std::string &e1, const std::string &e2)
{
  // ensure signle order, that ensurtes best performance
  if( e1.length()>e2.length() )
    return compare(e2, e1);
  assert( e1.length()<=e2.length() );

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

} // namespace Diff
} // namespace Algo
