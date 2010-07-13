/*
 * ParseTriggers.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseTriggers.hpp"

namespace ConfigIO
{

ParseTriggers::ParseTriggers(const XML::Node &node):
  Generic::NamedParse<TriggerConfig, TriggersConfigCollection>(node)
{
  assert( node.getName()=="triggers" );
}

} // namespace ConfigIO
