/*
 * Downloader.cpp
 *
 */
#include <boost/lexical_cast.hpp>

#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "Filter/IPBlackList/Downloader.hpp"

using namespace std;

namespace Filter
{
namespace IPBlackList
{

Downloader::Downloader(const unsigned int limit)
{
  const std::string     url( "http://www.dshield.org/ipsascii.html?limit=" +
                             boost::lexical_cast<string>(limit) );
  const curlpp::Cleanup myCleanup=curlpp::Cleanup();
  try
  {
    ss_ << curlpp::options::Url(url);
  }
  catch(const curlpp::RuntimeError &ex)
  {
    throw ExceptionDownloadFailed(SYSTEM_SAVE_LOCATION, ex.what() );
  }
  catch(const curlpp::LogicError &ex)
  {
    throw ExceptionDownloadFailed(SYSTEM_SAVE_LOCATION, ex.what() );
  }
}


} // namespace IPBlackList
} // namespace Filter
