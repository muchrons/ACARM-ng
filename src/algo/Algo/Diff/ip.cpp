/*
 * ip.cpp
 *
 */
#include <algorithm>
#include <cmath>

#include "Algo/Diff/ip.hpp"

namespace Algo
{
namespace Diff
{

namespace
{
template<typename T>
uint32_t getFirstBytesImpl(const T &ip)
{
  typedef typename T::bytes_type Tab;
  const Tab &t    =ip.to_bytes();
  const int  count=std::min<int>(3, t.size());
  int        pos  =0;
  uint32_t   out  =0;
  // read 'count' bytes from the address' end
  for(typename Tab::const_reverse_iterator it=t.rbegin(); it!=t.rend() && pos<count; ++it, ++pos)
    out|=((*it)&0xFF)<<(8*pos);
  // return the result
  assert(out<=0xFFFFFF);
  return out;
} // getFirstBytesImpl()

uint32_t getFirstBytes(const boost::asio::ip::address &ip)
{
  if( ip.is_v4() )
    return getFirstBytesImpl( ip.to_v4() );
  assert( ip.is_v6() && "unknown protocol version" );
  return getFirstBytesImpl( ip.to_v6() );
} // getFirstBytes()
} // unnamed namespace


Similarity compare(const boost::asio::ip::address &e1, const boost::asio::ip::address &e2)
{
  // if different types, return no similarity at all.
  if( e1.is_v4()!=e2.is_v4() || e1.is_v6()!=e2.is_v6() )
    return 0;

  const uint32_t diff=abs( getFirstBytes(e1) - getFirstBytes(e2) );
  return Similarity( 1.0/(1+diff/32.0) );
} // compare()

} // namespace Diff
} // namespace Algo
