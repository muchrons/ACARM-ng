/*
 * Graph.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Stubs/Graph.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

Graph::Graph(const Transaction &t, int handler):
  IO::Graph(t)
{
  assert(handler==42);
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
