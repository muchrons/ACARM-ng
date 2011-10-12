/*
 * LocalAnalyzersCache.cpp
 *
 */
#include <sstream>

#include "Commons/computeHash.hpp"
#include "Persistency/Exception.hpp"
#include "DataFacades/detail/LocalAnalyzersCache.hpp"

using namespace std;

namespace Persistency
{
namespace Facades
{
namespace detail
{

namespace
{
// removes NULLs
const char *fixNulls(const char *in)
{
  if(in==NULL)
    return "<NULL>";
  return in;
} // cstrHash()
} // unnamed namespace


LocalAnalyzersCache::Hash LocalAnalyzersCache::makeHash(const Analyzer::Name            &name,
                                                        const Analyzer::Version         &version,
                                                        const Analyzer::OperatingSystem &os,
                                                        const Analyzer::IP              *ip) const
{
  std::stringstream ss;
  ss << "Anlzr:HASH#";
  ss << fixNulls( name.get() );
  ss << "/";
  ss << fixNulls( version.get() );
  ss << "/";
  ss << fixNulls( os.get() );
  ss << "/";
  if(ip==NULL)
    ss << fixNulls(NULL);
  else
    ss << ip->to_string();

  return Commons::computeHash( ss.str() );
}

} // namespace detail
} // namespace Facades
} // namespace Persistency
