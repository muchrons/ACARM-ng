/*
 * ParseTriggers.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseTriggers.hpp"

namespace ConfigIO
{

ParseTriggers::ParseTriggers(const XML::Node &node):
  Generic::Parse<TriggerConfig, TriggersConfigCollection, true>(node)
{
  assert( node.getName()=="triggers" );
}

} // namespace ConfigIO
