/*
 * ParseLoggerNodes.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSELOGGERNODES_HPP_FILE
#define INCLUDE_CONFIGIO_PARSELOGGERNODES_HPP_FILE

/* pulic header */

#include "XML/Node.hpp"
#include "ConfigIO/LoggerNodes.hpp"
#include "ConfigIO/Exception.hpp"

// TODO: test
// TODO: comment

namespace ConfigIO
{

class ParseLoggerNodes
{
public:
  explicit ParseLoggerNodes(const XML::Node &node);

  const LoggerNodeConfig &getDefaultNodeConfig(void) const
  {
    return defaultNode_;
  }

  const LoggerNodes &getNodes(void) const
  {
    return nodes_;
  }

private:
  LoggerNodeConfig parseDefaultNode(const XML::Node &node) const;

  LoggerNodes parse(const XML::Node &node) const;

  void addParsed(LoggerNodes::Nodes &out,
                 const std::string  &parent,
                 const XML::Node    &node,
                 const char         *sep) const;

  LoggerNodeConfig defaultNode_;
  LoggerNodes      nodes_;
}; // class ParseLoggerNodes

} // namespace ConfigIO

#endif
