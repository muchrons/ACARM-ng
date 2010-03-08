/*
 * CachedEntry.hpp
 *
 */
#include "Filter/DNSResolver/CachedEntry.hpp"
#include <boost/asio.hpp>

using namespace boost::asio;

namespace Filter
{
namespace DNSResolver
{

void CachedEntry::resolve(const IP &ip)
{
  io_service                  ios;
  ip::tcp::endpoint           ep(ip, 0);
  ip::tcp::resolver           resolver(ios);
  ip::tcp::resolver::iterator it=resolver.resolve(ep);
  ip::tcp::resolver::iterator end;
  if( it==end || it->host_name()==ip.to_string() )
  {
    hasName_=false;
  }
  else
  {
    hasName_=true;
    name_   =it->host_name();
  }
}

} // namespace DNSResolver
} // namespace Filter
