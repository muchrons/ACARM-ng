/*
 * NodesFifo.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_NODESFIFLO_HPP_FILE
#define INCLUDE_CORE_TYPES_NODESFIFLO_HPP_FILE

/* public header */

#include "Base/Threads/UniqueFifoQueue.hpp"
#include "Persistency/GraphNode.hpp"

namespace Core
{
namespace Types
{

/** \brief fifo queue holding graph nodes.
 */
typedef Base::Threads::UniqueFifoQueue<Persistency::GraphNodePtrNN> NodesFifo;

} // namespace Types
} // namespace Core

#endif
