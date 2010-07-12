/*
 * QueueRestorer.hpp
 *
 */
#ifndef INCLUDE_CORE_QUEUERESTORER_HPP_FILE
#define INCLUDE_CORE_QUEUERESTORER_HPP_FILE

/* public header */

#include "Core/Types/SignedNodesFifo.hpp"

namespace Core
{

/** \brief class restoring data base's content (meta-alerts in use) to queue.
 */
class QueueRestorer: private boost::noncopyable
{
public:
  // TODO: this class should take IO::Connection and IO::Transaction as parameters!
  /** \brief restores data base content.
   *  \param queue output queue to write read meta-alerts to.
   */
  explicit QueueRestorer(Core::Types::SignedNodesFifo &queue);
}; // class QueueRestorer

} // namespace Core

#endif
