/*
 * AlertsFifo.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_ALERTSFIFLO_HPP_FILE
#define INCLUDE_CORE_TYPES_ALERTSFIFLO_HPP_FILE

/* public header */

#include "Base/Threads/LimitedQueue.hpp"
#include "Persistency/Alert.hpp"

namespace Core
{
namespace Types
{

/** \brief input fifo queue type holding incomming alerts.
 */
typedef Base::Threads::LimitedQueue<Persistency::AlertPtrNN, 32> AlertsFifo;    // TODO: alerts' queue size should be get from compile-time config.

} // namespace Types
} // namespace Core

#endif
