/*
 * OnProcess.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONPROCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONPROCESS_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Process.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief handle processing of process objects.
 */
struct OnProcess: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Process &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "process");

    ++p;

    if(p.get()=="path")
      return MainDispatcher::process(e.getPath().get(), p);
    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);
    if(p.get()=="md5")
      return MainDispatcher::process(e.getMD5(), p);
    if(p.get()=="pid")
      return MainDispatcher::process(e.getPID(), p);
    if(p.get()=="uid")
      return MainDispatcher::process(e.getUID(), p);
    if(p.get()=="username")
      return MainDispatcher::process(e.getUsername().get(), p);
    if(p.get()=="parameters")
      return MainDispatcher::process(e.getParameters(), p);
    if(p.get()=="referenceurl")
      return MainDispatcher::process(e.getReferenceURL(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnProcess

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
