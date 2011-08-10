/*
 * OnHost.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONHOST_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONHOST_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
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
  static bool process(const Host &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfLast(SYSTEM_SAVE_LOCATION, p);

    ++p;    // can be "host" or colleciton index

    if(p.get()=="ip")
      return MainDispatcher::process(e.getIP(), p);
    if(p.get()=="netmask")
      return MainDispatcher::process(e.getNetmask(), p);
    if(p.get()=="operatingsystem")
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
} // namespace Facades
} // namespace Persistency

#endif
