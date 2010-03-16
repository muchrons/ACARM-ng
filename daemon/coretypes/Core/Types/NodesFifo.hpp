/*
 * NodesFifo.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_NODESFIFLO_HPP_FILE
#define INCLUDE_CORE_TYPES_NODESFIFLO_HPP_FILE

/* public header */

#include "Base/Threads/FifoQueue.hpp"
#include "Persistency/GraphNode.hpp"

namespace Core
{
namespace Types
{

/** \brief fifo queue holding graph nodes.
 *  \note this class may be updated later on to insert only unique entries,
 *        since repeated ones does not introduce anything new.
 */
typedef Base::Threads::FifoQueue<Persistency::GraphNodePtrNN> NodesFifo;

} // namespace Types
} // namespace Core

#endif
