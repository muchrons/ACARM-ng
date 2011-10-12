/*
 * OnReferenceURL.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ONREFERENCEURL_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ONREFERENCEURL_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "DataFacades/StrAccess/MainDispatcher.hpp"

namespace Facades
{
namespace StrAccess
{

/** \brief handle processing reference url objects.
 */
struct OnReferenceURL: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Persistency::ReferenceURL &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "referenceurl");

    ++p;

    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);
    if(p.get()=="url")
      return MainDispatcher::process(e.getURL().get(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnReferenceURL

} // namespace StrAccess
} // namespace Facades

#endif
