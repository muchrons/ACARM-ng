/*
 * AlertsFifo.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_ALERTSFIFLO_HPP_FILE
#define INCLUDE_CORE_TYPES_ALERTSFIFLO_HPP_FILE

/* public header */

#include "Base/Threads/LimitedQueue.hpp"
#include "Persistency/Alert.hpp"
#include "ConfigConsts/inputs.hpp"

namespace Core
{
namespace Types
{

/** \brief input fifo queue type holding incomming alerts.
 */
typedef Base::Threads::LimitedQueue<Persistency::AlertPtrNN, ConfigConsts::inputQueueSize> AlertsFifo;

} // namespace Types
} // namespace Core

#endif
