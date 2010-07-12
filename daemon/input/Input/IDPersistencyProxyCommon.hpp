/*
 * IDPersistencyProxyCommon.hpp
 *
 */
#ifndef INCLUDE_INPUT_IDPERSISTENCYPROXYCOMMON_HPP_FILE
#define INCLUDE_INPUT_IDPERSISTENCYPROXYCOMMON_HPP_FILE

/* public header */

#include <string>

#include "Persistency/Analyzer.hpp"
#include "Input/detail/IDPersistencyProxyBase.hpp"

namespace Input
{

/** \brief object responsible for I/O operations NOT related to
 *         any speicifc inputs (i.e. common settings).
 */
class IDPersistencyProxyCommon: private detail::IDPersistencyProxyBase
{
public:
  /** \brief create object that operates on given input's paramters.
   *  \param conn      connection to persistency, to be used.
   *  \param t         transaction to operate on.
   */
  IDPersistencyProxyCommon(Persistency::IO::ConnectionPtrNN  conn,
                           Persistency::IO::Transaction     &t);

  /** \brief read next free ID.
   *  \return ID saved as a last free one.
   */
  Persistency::Analyzer::ID readNextFreeID(void);
  /** \brief save next free ID.
   *  \param nextFreeID free ID to be saved.
   */
  void saveNextFreeID(Persistency::Analyzer::ID nextFreeID);
}; // class IDPersistencyProxyCommon

} // namespace Input

#endif
