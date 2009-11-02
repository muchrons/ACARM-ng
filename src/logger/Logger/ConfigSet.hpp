/*
 * ConfigSet.hpp
 *
 */
#ifndef INCLUDE_LOGGER_CONFIGSET_HPP_FILE
#define INCLUDE_LOGGER_CONFIGSET_HPP_FILE

#include "Logger/NodeName.hpp"
#include "Logger/NodeConf.hpp"

// TODO: test
// TODO: implement
// TODO: comment

namespace Logger
{

struct ConfigSet
{
  static NodeConfPtr getConfig(const NodeName &nn);
}; // struct ConfigSet

} // namespace Logger

#endif
