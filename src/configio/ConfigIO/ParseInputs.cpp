/*
 * ParseInputs.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseInputs.hpp"

namespace ConfigIO
{

ParseInputs::ParseInputs(const XML::Node &node):
  Generic::NamedParse<InputConfig, InputsConfigCollection>(node)
{
  assert( node.getName()=="inputs" );
}

} // namespace ConfigIO
