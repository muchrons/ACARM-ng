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
  try
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
      name_   =it->host_name();
      hasName_=true;
    }
  }
  catch(const boost::system::system_error &ex)
  {
    throw ExceptionUnableToResolve(SYSTEM_SAVE_LOCATION, ip.to_string(), ex.what() );
  }
  catch(const std::exception &ex)
  {
    throw ExceptionUnableToResolve(SYSTEM_SAVE_LOCATION, ip.to_string(), ex.what() );
  }
}

} // namespace DNSResolver
} // namespace Filter
