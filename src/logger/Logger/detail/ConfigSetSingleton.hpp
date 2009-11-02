/*
 * ConfigSetSingleton.hpp
 *
 */
#ifndef INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE
#define INCLUDE_LOGGER_DETAIL_CONFIGSETSINGLETON_HPP_FILE

#include "System/Singleton.hpp"
#include "Logger/NodeName.hpp"
#include "Logger/NodeConf.hpp"

// TODO: test
// TODO: implement
// TODO: comment

namespace Logger
{
namespace detail
{

class ConfigSetImpl
{
public:
  NodeConfPtr getNodeConfiguration(const NodeName &nn);

private:
  ConfigSetImpl();

  friend class System::Singleton<detail::ConfigSetImpl>;
}; // class ConfigSet


typedef System::Singleton<detail::ConfigSetImpl> ConfigSetSingleton;
} // namespace detail

} // namespace Logger

#endif
