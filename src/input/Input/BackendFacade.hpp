/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_INPUT_BACKENDFACADE_HPP_FILE
#define INCLUDE_INPUT_BACKENDFACADE_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Core/Types/BackendFacade.hpp"

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
   *  \param conn           connection object to use.
   *  \param name           name of input this object is created for.
   *  \param creator        creator of analyzers.
   *  \param heartbeatOwner owner name to be used for heartbeats.
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN          conn,
                const std::string                        &name,
                Persistency::Facades::AnalyzersCreator   &creator,
                const Persistency::IO::Heartbeats::Owner &heartbeatOwner);

  /** \brief gets mapping from given originalID to analyzer. if ID's not mapped,
   *         new entry's created and returned.
   *  \param name       analyzer's name.
   *  \param version    analyzer's version.
   *  \param os         analyzer's OS.
   *  \param ip         analyzer's IP.
   *  \return analyzer's object instance.
   */
  Persistency::AnalyzerPtrNN getAnalyzer(const Persistency::Analyzer::Name            &name,
                                         const Persistency::Analyzer::Version         &version,
                                         const Persistency::Analyzer::OperatingSystem &os,
                                         const Persistency::Analyzer::IP              *ip);

  /** \brief send heartbeat, that arrived from an external source (module).
   *  \param m        module's name.
   *  \param deadline maximum ammount of time to wait for next heartbeat.
   *
   *  \note this facility should be used for incomming heartbeats onyl, since heartbeating
   *        for user's implementation (i.e. The Input) is done automatically.
   */
  void heartbeat(const Persistency::IO::Heartbeats::Module &m, unsigned int deadline);

private:
  Logger::Node                              log_;
  Persistency::Facades::AnalyzersCreator   &creator_;
  const Persistency::IO::Heartbeats::Owner  heartbeatOwner_;
}; // class BackendFacade

} // namespace Input

#endif
