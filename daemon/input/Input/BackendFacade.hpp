/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_INPUT_BACKENDFACADE_HPP_FILE
#define INCLUDE_INPUT_BACKENDFACADE_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Core/Types/BackendFacade.hpp"

// TODO

namespace Input
{
/** \brief persistent storage communication simplification layer.
 *
 * end user wrapper for communication with persistency storage. it updates data
 * of object it works on and saves changes to persistency at a time.
 *
 * \note this object allows transaction's to take place, but only on persistent
 *       level, i.e. rollbacking transaction does not change user objects.
 */
class BackendFacade: public Core::Types::BackendFacade
{
public:
  /** \brief create object's instance.
   *  \param conn connection object to use.
   *  \param name name of filter this object is created for.
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                const std::string                &name);


private:
}; // class BackendFacade

} // namespace Input

#endif
