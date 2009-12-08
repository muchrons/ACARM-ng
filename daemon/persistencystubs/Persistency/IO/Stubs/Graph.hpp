/*
 * Graph.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_GRAPH_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_GRAPH_HPP_FILE

#include "Persistency/IO/Graph.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief Graph's stub
 */
class Graph: public IO::Graph
{
public:
  explicit Graph(const Transaction &t, int handler);
  // TODO
}; // class Graph

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
