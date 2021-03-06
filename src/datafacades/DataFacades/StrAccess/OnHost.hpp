/*
 * OnHost.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ONHOST_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ONHOST_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Host.hpp"
#include "DataFacades/StrAccess/MainDispatcher.hpp"

namespace DataFacades
{
namespace StrAccess
{

/** \brief handle processing of host objects.
 */
struct OnHost: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Persistency::Host &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "host");

    ++p;

    if(p.get()=="ip")
      return MainDispatcher::process(e.getIP(), p);
    if(p.get()=="netmask")
      return MainDispatcher::process(e.getNetmask(), p);
    if(p.get()=="os")
      return MainDispatcher::process(e.getOperatingSystem().get(), p);
    if(p.get()=="referenceurl")
      return MainDispatcher::process(e.getReferenceURL(), p);
    if(p.get()=="services")
      return MainDispatcher::process(e.getServices(), p);
    if(p.get()=="processes")
      return MainDispatcher::process(e.getProcesses(), p);
    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnHost

} // namespace StrAccess
} // namespace DataFacades

#endif
