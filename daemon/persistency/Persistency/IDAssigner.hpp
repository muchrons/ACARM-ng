/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{

/** \brief implementation that uses DynamicConfig to communicate with persistent storage.
 */
struct IDAssigner: private boost::noncopyable
{
  /** \brief get next ID value to be used
   *  \param dc dynamic configuration reader/writer to be used.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  virtual MetaAlert::ID assign(IO::DynamicConfig &dc);
}; // struct IDAssigner

} // namespace Persistency

#endif
