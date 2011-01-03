/*
 * PluginsLoader.hpp
 *
 */
#ifndef INCLUDE_CORE_PLUGINSLOADER_HPP_FILE
#define INCLUDE_CORE_PLUGINSLOADER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"

namespace Core
{

/** \brief loader for plugins.
 *
 *  this class wraps all functionality related to reading plugins
 *  and loading them into the system's core.
 */
class PluginsLoader: private boost::noncopyable
{
public:
  /** \brief load all plugins.
   */
  PluginsLoader(void);
  /** \brief destroys this object.
   */
  ~PluginsLoader(void);

private:
  Logger::Node log_;
}; // class PluginsLoader

} // namespace Core

#endif
