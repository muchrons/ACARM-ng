/*
 * Loader.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_LOADER_HPP_FILE
#define INCLUDE_PLUGINS_LOADER_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include "System/Plugins/Builder.hpp"
#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
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
 *
 *  in order to be valid plugin, one must provide following function:
 *  <code>
 *    char *register_plugin(void *dynObj);
 *  </code>
 *  that registers plugin to a proper factory using Plugins::Registrator<>
 *  helper template. pointer argument given to a call is valid pointer
 *  to Plugins::DynamicObject instance, related with this plugin (converted
 *  to void* because of the C language limitations). returned value is
 *  either NULL (i.e. no error) or malloc-ed, C-stype string with error
 *  description. not that this string is returned by pointer, thus must always
 *  be valid and accessible. to achieve this use strdup(), like this:
 *  <code>
 *    return strdup("my error description");  // 1
 *    return strdup( someStdString.c_str() ); // 2
 *  </code>
 */
class Loader: private boost::noncopyable
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
  typedef std::vector<System::Plugins::DynamicObject> DeinitSet;

  void loadAll(const boost::filesystem::path &dir);
  void loadPlugin(const boost::filesystem::path &plugin, DeinitSet &deinitSet);

  Logger::Node             log_;
  System::Plugins::Builder builder_;
  size_t                   count_;
}; // class Loader

} // namespace Plugins

#endif
