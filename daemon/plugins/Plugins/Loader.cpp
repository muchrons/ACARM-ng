/*
 * Loader.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Plugins/Loader.hpp"
#include "Plugins/Registrator.hpp"

using namespace Commons::Filesystem;
namespace fs=boost::filesystem;

namespace Plugins
{

Loader::Loader(const boost::filesystem::path &dir):
  log_("plugins.loader")
{

  LOGMSG_INFO_S(log_)<<"loading plugins from directory '"<<dir.string()<<"'";
  const fs::directory_iterator end=fs::directory_iterator();
  // loop thought all elements in the directory
  int count=0;
  for(fs::directory_iterator it(dir); it!=end; ++it)
  {
    const fs::path p=it->path();
    if( !isFileSane(p) )
    {
      LOGMSG_DEBUG_S(log_)<<"file does not look sane: '"<<p<<"'";
      continue;
    }

    // ok - if all the basic conditions are meet, load the plugin
    loadPlugin(p);
    ++count;
  } // for(files)

  LOGMSG_INFO_S(log_)<<count<<" plugins loaded";
}

void Loader::loadPlugin(const fs::path &plugin)
{
  //Registrator reg( builder_.open(plugin) );
  // TODO
}

} // namespace Plugins
