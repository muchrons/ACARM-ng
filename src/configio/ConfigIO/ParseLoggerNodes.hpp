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


namespace ConfigIO
{

/** \brief class for parsing nodes configuration part of logger config.
 */
class ParseLoggerNodes
{
public:
  /** \brief creates class and saves parsing results.
   *  \param node node to start parsing from.
   */
  explicit ParseLoggerNodes(const XML::Node &node);

  /** \brief gets configuration of default (top most) node.
   *  \return configuration of default node.
   */
  const LoggerNodeConfig &getDefaultNodeConfig(void) const
  {
    return defaultNode_;
  }

  /** \brief gets configuration of definied nodes.
   *  \return nodes' configuration.
   */
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
