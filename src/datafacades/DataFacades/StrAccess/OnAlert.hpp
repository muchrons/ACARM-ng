/*
 * OnAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONALERT_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Alert.hpp"
#include "DataFacades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief handle processing of alert objects.
 */
struct OnAlert: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Alert &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "alert");

    ++p;

    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);
    if(p.get()=="analyzers")
      return MainDispatcher::process(e.getAnalyzers(), p);
    if(p.get()=="detected")
    {
      const Timestamp *ptr=e.getDetectionTime();
      if(ptr!=NULL)
        return MainDispatcher::process(ptr->get(), p);
      else
        return MainDispatcher::process(ptr, p);
    }
    if(p.get()=="created")
      return MainDispatcher::process(e.getCreationTime().get(), p);
    if(p.get()=="severity")
      return MainDispatcher::process(e.getSeverity().getName(), p);
    if(p.get()=="certainty")
      return MainDispatcher::process(e.getCertainty().get(), p);
    if(p.get()=="description")
      return MainDispatcher::process(e.getDescription(), p);
    if(p.get()=="sources")
      return MainDispatcher::process(e.getSourceHosts(), p);
    if(p.get()=="targets")
      return MainDispatcher::process(e.getTargetHosts(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnAlert

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
