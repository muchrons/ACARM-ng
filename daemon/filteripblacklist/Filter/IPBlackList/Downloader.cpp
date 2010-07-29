/*
 * Downloader.cpp
 *
 */
#include <sstream>

#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "Commons/Convert.hpp"
#include "Filter/IPBlackList/Downloader.hpp"

using namespace std;

namespace Filter
{
namespace IPBlackList
{

Downloader::Downloader(const unsigned int limit):
  url_( "http://www.dshield.org/ipsascii.html?limit=" +
        Commons::Convert::to<string>(limit) )
{
}

std::string Downloader::download(void) const
{
  stringstream ss;
  try
  {
    ss<<curlpp::options::Url(url_);
  }
  catch(const curlpp::RuntimeError &ex)
  {
    throw ExceptionDownloadFailed(SYSTEM_SAVE_LOCATION, ex.what() );
  }
  catch(const curlpp::LogicError &ex)
  {
    throw ExceptionDownloadFailed(SYSTEM_SAVE_LOCATION, ex.what() );
  }
  return ss.str();
}

} // namespace IPBlackList
} // namespace Filter
