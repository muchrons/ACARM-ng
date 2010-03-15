/*
 * Downloader.cpp
 *
 */
#include <boost/lexical_cast.hpp>

#include "curlpp/curlpp.hpp"
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
  ss_ << curlpp::options::Url(url);
  // catch: curlpp::LogicError and curlpp::RuntimeError
}


} // namespace IPBlackList
} // namespace Filter
