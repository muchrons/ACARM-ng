/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_INPUT_BACKENDFACADE_HPP_FILE
#define INCLUDE_INPUT_BACKENDFACADE_HPP_FILE

/* public header */

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
   *  \param conn         connection object to use.
   *  \param name         name of input this object is created for.
   *  \param analyzersMap map of ID->analyzer (internal input's cache).
   *  \param creator      creator of analyzers.
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN        conn,
                const std::string                      &name,
                Persistency::Facades::AnalyzersCreator &creator);

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

private:
  Persistency::Facades::AnalyzersCreator &creator_;
}; // class BackendFacade

} // namespace Input

#endif
