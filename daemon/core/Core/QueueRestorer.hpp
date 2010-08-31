/*
 * QueueRestorer.hpp
 *
 */
#ifndef INCLUDE_CORE_QUEUERESTORER_HPP_FILE
#define INCLUDE_CORE_QUEUERESTORER_HPP_FILE

/* public header */

#include "Persistency/IO/Connection.hpp"
#include "Core/Types/SignedNodesFifo.hpp"

namespace Core
{

/** \brief class restoring data base's content (meta-alerts in use) to queue.
 */
class QueueRestorer: private boost::noncopyable
{
public:
  /** \brief restores data base content.
   *  \param conn  connection to be used for restoring.
   *  \param queue output queue to write read meta-alerts to.
   */
  QueueRestorer(Persistency::IO::ConnectionPtrNN conn, Core::Types::SignedNodesFifo &queue);
}; // class QueueRestorer

} // namespace Core

#endif
