/*
 * ParseTriggers.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSETRIGGERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSETRIGGERS_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/TriggerConfig.hpp"
#include "ConfigIO/TriggersConfigCollection.hpp"


namespace ConfigIO
{

/** \brief parser for triggers' configuration.
 */
class ParseTriggers
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseTriggers(const XML::Node &node);

  /** \brief gets triggers' configuration.
   *  \return collection of striggers' configuration.
   */
  const TriggersConfigCollection &getConfig(void) const
  {
    return tcc_;
  }

private:
  void parse(const XML::Node &node);

  TriggersConfigCollection tcc_;
}; // class ParaseTriggers

} // namespace ConfigIO

#endif
