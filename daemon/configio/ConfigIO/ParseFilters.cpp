/*
 * ParseFilters.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseFilters.hpp"

namespace ConfigIO
{

ParseFilters::ParseFilters(const XML::Node &node):
  Generic::Parse<FilterConfig, FiltersConfigCollection>(node)
{
  assert( node.getName()=="filters" );
}
} // namespace ConfigIO
