/*
 * IDPersistencyProxySpecific.hpp
 *
 */
#ifndef INCLUDE_INPUT_IDPERSISTENCYPROXYSPECIFIC_HPP_FILE
#define INCLUDE_INPUT_IDPERSISTENCYPROXYSPECIFIC_HPP_FILE

#include <string>

#include "Persistency/Analyzer.hpp"
#include "Input/detail/IDPersistencyProxyBase.hpp"

namespace Input
{

/** \brief object responsible for I/O operations related to
 *         speicifc inputs (i.e. non-common settings).
 */
class IDPersistencyProxySpecific: private detail::IDPersistencyProxyBase
{
public:
  /** \brief create object that operates on given input's paramters.
   *  \param conn      connection to persistency, to be used.
   *  \param t         transaction to operate on.
   *  \param inputName name of the input to work for.
   */
  IDPersistencyProxySpecific(Persistency::IO::ConnectionPtrNN  conn,
                             Persistency::IO::Transaction     &t,
                             const std::string                &inputName);

  /** \brief request to save mapping inputID->id.
   *  \param inputID key of mapping.
   *  \param id      value of mapping.
   */
  void saveMapping(const std::string &inputID, Persistency::Analyzer::ID id);
}; // class IDPersistencyProxySpecific

} // namespace Input

#endif
