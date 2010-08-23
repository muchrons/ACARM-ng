/*
 * IDAssignerDynamic.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNERDYNAMIC_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNERDYNAMIC_HPP_FILE

/* public header */

#include "Persistency/MetaAlert.hpp"

namespace Persistency
{

/** \brief implementation that uses DynamicConfig to communicate with persistent storage.
 */
struct IDAssignerDynamic: public MetaAlert::IDAssigner
{
  /** \brief get next ID value to be used
   *  \param dc dynamic configuration reader/writer to be used.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  virtual MetaAlert::ID assign(IO::DynamicConfig &dc);
}; // struct IDAssignerDynamic

} // namespace Persistency

#endif
