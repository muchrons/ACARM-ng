/*
 * ParseGeneralConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEGENERALCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEGENERALCONFIG_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/GeneralConfig.hpp"
#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{

/** \brief parser for GeneralConfig's paramerters.
 */
class ParseGeneralConfig
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseGeneralConfig(const XML::Node &node);

  /** \brief gets general configuration.
   *  \return general configuration paramters
   */
  const GeneralConfig &getConfig(void) const
  {
    return gc_;
  }

private:
  GeneralConfig gc_;
}; // class ParseGeneralConfig

} // namespace ConfigIO

#endif
