/*
 * OnService.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONSERVICE_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONSERVICE_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Service.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief handle processing service objects.
 */
struct OnService: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Service &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "service");

    ++p;

    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);
    if(p.get()=="port")
      return MainDispatcher::process(e.getPort().get(), p);
    if(p.get()=="protocol")
      return MainDispatcher::process(e.getProtocol().get(), p);
    if(p.get()=="referenceurl")
      return MainDispatcher::process(e.getReferenceURL(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnService

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
