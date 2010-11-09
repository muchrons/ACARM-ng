/*
 * Config.cpp
 *
 */
#include "Commons/Filesystem/isFileSane.hpp"
#include "Trigger/ExtApp/Config.hpp"

using namespace boost::filesystem;

namespace Trigger
{
namespace ExtApp
{

namespace
{
path checkAndCannonicalizePath(const path &p)
{
  // check if file is sane
  if( Commons::Filesystem::isFileSane(p)==false )
    throw Exception(SYSTEM_SAVE_LOCATION, "invalid executable path: " + p.string() );

  return system_complete(p);
} // checkAndCannonicalizePath()
} // unnamed namespace


Config::Config(const Path &path, const Simple::ThresholdConfig &th):
  th_(th),
  path_( checkAndCannonicalizePath(path) )
{
}

const Simple::ThresholdConfig &Config::getThresholdConfig(void) const
{
  return th_;
}

const Config::Path &Config::getPath(void) const
{
  return path_;
}


} // namespace ExtApp
} // namespace Trigger
