/*
 * ConfigSetSingleton.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE

#include <boost/noncopyable.hpp>

#include "System/Singleton.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Logger/NodeName.hpp"
#include "Logger/NodeConf.hpp"
#include "Logger/AppenderMap.hpp"

namespace Logger
{
namespace detail
{

/** \brief implenentation of configuration singleton.
 */
class ConfigSetImpl: private boost::noncopyable
{
public:
  /** \brief returns configariotn for a gienve node.
   *  \param nn node to return configuration for.
   *  \return given node's configuration.
   *  \note if configurations are pre-cached so that asking twice for the same
   *        node's config, will return exactly the same structure.
   */
  NodeConfPtr getNodeConfiguration(const NodeName &nn);

private:
  //
  // private c-tor and friend-singleton-template ensures this class will be
  // created in one instance, by singleton class.
  //
  ConfigSetImpl();
  friend class System::Singleton<detail::ConfigSetImpl>;

  //
  // NOTE: solution with configration maps and returning pointer to
  //       configurations looks strange at the first sight, but it is done
  //       that way for a very specific reason. this allows thread-safe update
  //       of loggers' configration on a fly. this feature is planned to be
  //       used later on to allow dynamic (runtime) change of configuration.
  //

  typedef std::map<std::string, NodeConfPtr> ConfigMap;

  ConfigMap            cfgMap_;
  AppenderMap          appMap_;
  Base::Threads::Mutex mutex_;
}; // class ConfigSet


/** \brief singleton type itself.
 */
typedef System::Singleton<detail::ConfigSetImpl> ConfigSetSingleton;

} // namespace detail
} // namespace Logger

#endif
