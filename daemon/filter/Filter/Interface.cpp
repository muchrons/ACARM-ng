/*
 * Interface.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/Interface.hpp"

namespace Filter
{

void Interface::process(Node n, NodesDeque &changed)
{
  BackendProxy bp( conn_, getFilterName() );

  assert( changed.size()==0 && "non-empty output collection received");
  processImpl(n, changed, bp);
}

Interface::Interface(const std::string &name):
  name_(name),
  conn_( Persistency::IO::create() )
{
}

} // namespace Filter
