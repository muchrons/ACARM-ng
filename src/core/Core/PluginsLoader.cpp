/*
 * PluginsLoader.cpp
 *
 */
#include "ConfigIO/Singleton.hpp"
#include "Logger/Logger.hpp"
#include "Plugins/Loader.hpp"
#include "Core/PluginsLoader.hpp"

using boost::filesystem::path;

namespace Core
{

PluginsLoader::PluginsLoader(void):
  log_("core.pluginsloader")
{
  const path pluginsDir=ConfigIO::Singleton::get()->generalConfig().getPluginsDir();
  LOGMSG_INFO_S(log_)<<"loading plugins from '"<<pluginsDir<<"' directory";
  Plugins::Loader loader(pluginsDir);       // load all plugins
  LOGMSG_INFO_S(log_)<<"all "<<loader.loadedCount()<<" plugins loaded and initialized successfully";
}

PluginsLoader::~PluginsLoader(void)
{
  LOGMSG_INFO_S(log_)<<"destroying... note that plugins will remain loaded until "
                       "the last object stops using it (i.e. holds reference to it)";
}

} // namespace Core
