/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_INPUT_BACKENDFACADE_HPP_FILE
#define INCLUDE_INPUT_BACKENDFACADE_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Core/Types/BackendFacade.hpp"
#include "Input/PersistencyProxy.hpp"
#include "Input/detail/AnalyzersMap.hpp"

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
class BackendFacade: public  Core::Types::BackendFacade,
                     private PersistencyProxy
{
public:
  /** \brief create object's instance.
   *  \param conn         connection object to use.
   *  \param name         name of filter this object is created for.
   *  \param analyzersMap map of ID->analyzer (internal input's cache).
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                const std::string                &name,
                detail::AnalyzersMap             &analyzersMap);

  /** \brief gets mapping from given originalID to analyzer. if ID's not mapped, new entry's added.
   *  \param originalID ID returned by input.
   *  \param name       analyzer's name.
   *  \param version    analyzer's version.
   *  \param os         analyzer's OS.
   *  \param ip         analyzer's IP.
   *  \return analyzer's object instance.
   */
  Persistency::AnalyzerPtrNN getAnalyzer(const std::string                            &originalID,
                                         const Persistency::Analyzer::Name            &name,
                                         const Persistency::Analyzer::Version         &version,
                                         const Persistency::Analyzer::OperatingSystem &os,
                                         const Persistency::Analyzer::IP              *ip);

private:
  // PersistencyProxy's method implementation
  virtual void saveMapping(const std::string &inputID, Persistency::Analyzer::ID id);

  detail::AnalyzersMap &analyzersMap_;
}; // class BackendFacade

} // namespace Input

#endif
