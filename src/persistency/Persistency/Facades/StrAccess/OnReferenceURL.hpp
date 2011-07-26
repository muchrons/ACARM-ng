/*
 * OnReferenceURL.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONREFERENCEURL_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONREFERENCEURL_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
// TODO

struct OnReferenceURL: private System::NoInstance
{
  template<typename TParams>
  static void get(const ReferenceURL &e, TParams &p)
  {
    typedef TParams<OnError>::type ErrH;

    ErrH::throwIfLast(SYSTEM_SAVE_LOCATION, p);

    if(p.get()=="name")
      return process( e.getName().get() );
    if(p.get()=="url")
      return process( e.getURL().get() );

    ErrH::throwInvalid(SYSTEM_SAVE_LOCATION, p);
  }
}; // struct OnReferenceURL

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
