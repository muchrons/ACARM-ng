/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_BACKENDFACADE_HPP_FILE
#define INCLUDE_TRIGGER_BACKENDFACADE_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Core/Types/Proc/BackendFacade.hpp"


namespace Trigger
{
/** \brief persistent storage communication simplification layer.
 *
 * end user wrapper for communication with persistency storage. it updates data
 * of object it works on and saves changes to persistency at a time.
 *
 * \note this object allows transaction's to take place, but only on persistent
 *       level, i.e. rollbacking transaction does not change user objects.
 */
class BackendFacade: public Core::Types::Proc::BackendFacade
{
public:
  /** \brief create object's instance.
   *  \param conn        connection object to use.
   *  \param triggerName name of trigger this object is created for.
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
               const std::string                &triggerName);

  /** \brief saves information that given meta-alert has been already
   *         triggered by this trigger.
   *  \param ma meta-alert that has been triggered.
   */
  void markAsTriggered(Persistency::MetaAlertPtrNN ma);
}; // class BackendFacade

} // namespace Trigger

#endif
