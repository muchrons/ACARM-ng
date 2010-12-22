/*
 * Loader.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_LOADER_HPP_FILE
#define INCLUDE_PLUGINS_LOADER_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "System/Plugins/Builder.hpp"
#include "Logger/Node.hpp"
#include "Plugins/ExceptionInvalidPlugin.hpp"
#include "Plugins/ExceptionInvalidDirectory.hpp"
#include "Plugins/ExceptionRegistrationError.hpp"

namespace Plugins
{

/** \brief plugins loader class.
 *
 *  loads all plugins to the system. plugins are responsible for proper
 *  registereing within ACARM-ng's environment. plugins will be automatically
 *  unregistered when they are no longer in use (this has nothing to do with
 *  life-time of this object).
 */
class Loader
{
public:
  /** \brief loads all plugins from a given directory.
   *  \param dir directory to load plugins from.
   */
  explicit Loader(const boost::filesystem::path &dir);

  /** \brief get loaded's plugins count.
   *  \return count of plugins.
   */
  size_t loadedCount(void) const
  {
    return count_;
  }

private:
  void loadAll(const boost::filesystem::path &dir);
  void loadPlugin(const boost::filesystem::path &plugin);

  Logger::Node             log_;
  System::Plugins::Builder builder_;
  size_t                   count_;
}; // class Loader

} // namespace Plugins

#endif
