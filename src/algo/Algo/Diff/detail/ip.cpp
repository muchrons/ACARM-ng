/*
 * ip.cpp
 *
 */
#include <algorithm>
#include <cmath>

#include "Algo/Diff/detail/ip.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

namespace
{

// number of bytes to get into consideration
const int g_split=3;


template<typename T>
uint32_t getLowBytes(const T &ip)
{
  typedef typename T::bytes_type Tab;
  const Tab &t    =ip.to_bytes();
  const int  count=std::min<int>(g_split, t.size());
  int        pos  =0;
  uint32_t   out  =0;
  // read 'count' bytes from the address' end
  for(typename Tab::const_reverse_iterator it=t.rbegin(); it!=t.rend() && pos<count; ++it, ++pos)
    out|=((*it)&0xFF)<<(8*pos);
  // return the result
  assert(out<=0xFFFFFF);
  return out;
} // getLowBits()


template<typename T>
bool highBytesDiffer(const T &ip1, const T &ip2)
{
  typedef typename T::bytes_type Tab;
  const Tab &t1   =ip1.to_bytes();
  const Tab &t2   =ip2.to_bytes();
  assert( t1.size()==t2.size() );
  const int  count=t1.size()-g_split;
  // check each high-byte dif difference
  for(int pos=0; pos<count; ++pos)
    if(t1[pos]!=t2[pos])
      return true;
  return false;
} // getLowBits()


template<typename T>
Similarity process(const T &ip1, const T &ip2)
{
  // if there is difference in MSBs, assume no similarity
  if( highBytesDiffer(ip1, ip2) )
    return Similarity(0);
  // if only low bytes differ, check the difference and compute similarity
  const uint32_t diff=abs( getLowBytes(ip1) - getLowBytes(ip2) );
  return Similarity( 1.0/(1+diff/32.0) );
} // process()


Similarity computeSimilarity(const boost::asio::ip::address &ip1, const boost::asio::ip::address &ip2)
{
  if( ip1.is_v4() )
  {
    // IPv4
    assert( ip2.is_v4() && "types differ" );
    return process( ip1.to_v4(), ip2.to_v4() );
  }
  // IPv6
  assert( ip1.is_v6() && "unknown protocol version" );
  assert( ip2.is_v6() && "types differ" );
  return process( ip1.to_v6(), ip2.to_v6() );
} // computeSimilarity()

} // unnamed namespace


Similarity compare(const boost::asio::ip::address &e1, const boost::asio::ip::address &e2)
{
  // speed up computations in optimistic case
  if(&e1==&e2)
    return 1;
  // if different types, return no similarity at all.
  if( e1.is_v4()!=e2.is_v4() || e1.is_v6()!=e2.is_v6() )
    return 0;
  // perform full check
  return computeSimilarity(e1, e2);
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo
