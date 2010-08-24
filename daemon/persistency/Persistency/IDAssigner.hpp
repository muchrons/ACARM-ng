/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{

/** \brief implementation that uses DynamicConfig to communicate with persistent storage.
 */
struct IDAssigner: private boost::noncopyable
{
  /** \brief create object that will assign new IDs.
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   */
  IDAssigner(IO::ConnectionPtrNN conn, IO::Transaction &t);

  /** \brief get next ID value to be used
   *  \return free ID value, that can be used for new MetaAlert.
   */
  MetaAlert::ID assign(void);

private:
  // TODO: add protection against multiple copyies of this object existing at
  //       the same time in program.
  IO::ConnectionPtrNN       conn_;
  IO::Transaction          &t_;
  IO::DynamicConfigAutoPtr  dc_;
}; // struct IDAssigner

} // namespace Persistency

#endif
