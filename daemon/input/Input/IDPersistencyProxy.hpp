/*
 * IDPersistencyProxy.hpp
 *
 */
#ifndef INCLUDE_INPUT_IDPERSISTENCYPROXY_HPP_FILE
#define INCLUDE_INPUT_IDPERSISTENCYPROXY_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Input
{

/** \brief object responsible for I/O operations related to mappings.
 */
class IDPersistencyProxy: private boost::noncopyable
{
public:
  /** \brief create object.
   *  \param conn connection to persistency, to be used.
   *  \param t    transaction to operate on.
   */
  IDPersistencyProxy(Persistency::IO::ConnectionPtrNN  conn,
                     Persistency::IO::Transaction     &t);

  /** \brief request to save mapping inputID->id.
   *  \param inputID key of mapping.
   *  \param id      value of mapping.
   */
  void saveMapping(const std::string &inputID, Persistency::Analyzer::ID id);

  /** \brief read next free ID.
   *  \return ID saved as a last free one.
   */
  Persistency::Analyzer::ID readNextFreeID(void);
  /** \brief save next free ID.
   *  \param nextFreeID free ID to be saved.
   */
  void saveNextFreeID(Persistency::Analyzer::ID nextFreeID);

private:
  Logger::Node                      log_;
  Persistency::IO::ConnectionPtrNN  conn_;
  Persistency::IO::Transaction     &t_;
}; // class IDPersistencyProxy

} // namespace Input

#endif
