/*
 * ConfigSetSingleton.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE

#include <boost/noncopyable.hpp>

#include "System/Singleton.hpp"
#include "Logger/NodeName.hpp"
#include "Logger/NodeConf.hpp"
#include "Logger/Appenders/Base.hpp"

// TODO: test
// TODO: comment

namespace Logger
{
namespace detail
{

class ConfigSetImpl: private boost::noncopyable
{
public:
  NodeConfPtr getNodeConfiguration(const NodeName &nn);

private:
  //
  // private c-tor and friend-singleton-template ensures this class will be
  // created in one instance, by singleton class.
  //
  ConfigSetImpl();
  friend class System::Singleton<detail::ConfigSetImpl>;

  typedef std::map<std::string, NodeConfPtr>        ConfigMap;
  typedef std::map<const char*, Appenders::BasePtr> AppendersMap;

  ConfigMap    cfgMap_;
  AppendersMap appMap_;
}; // class ConfigSet


typedef System::Singleton<detail::ConfigSetImpl> ConfigSetSingleton;
} // namespace detail

} // namespace Logger

#endif
