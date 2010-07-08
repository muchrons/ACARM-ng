/*
 * SignedNodesFifo.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_SIGNEDNODESFIFLO_HPP_FILE
#define INCLUDE_CORE_TYPES_SIGNEDNODESFIFLO_HPP_FILE

/* public header */

#include "Base/Threads/FifoQueue.hpp"
#include "Core/Types/SignedNode.hpp"

namespace Core
{
namespace Types
{

/** \brief fifo queue holding nodes allong with additional data.
 */
typedef Base::Threads::FifoQueue<SignedNode> SignedNodesFifo;

} // namespace Types
} // namespace Core

#endif
