/*
 * ConfigSet.hpp
 *
 */
#ifndef INCLUDE_LOGGER_CONFIGSET_HPP_FILE
#define INCLUDE_LOGGER_CONFIGSET_HPP_FILE

#include "Logger/NodeName.hpp"
#include "Logger/NodeConf.hpp"

namespace Logger
{

/** \brief singleton's frontend for getting configuration.
 */
struct ConfigSet
{
  /** \brief returns configuration for a given node.
   *  \param nn node to return configration for.
   *  \return configration for node given as a paramter.
   */
  static NodeConfPtr getConfig(const NodeName &nn);
}; // struct ConfigSet

} // namespace Logger

#endif
