/*
 * CachedEntry.hpp
 *
 */
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Filter/DNSResolver/CachedEntry.hpp"

using namespace boost::asio;

namespace Filter
{
namespace DNSResolver
{

void CachedEntry::resolve(const IP &ip)
{
  // TODO: N5boost6system12system_errorE is thrown here on error - rethrow it as
  //       internal DNS-resolv-ailure error, to be more specific about the problem
  //       by type (N5boost6system12system_errorE is too generic to catch later on).
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
