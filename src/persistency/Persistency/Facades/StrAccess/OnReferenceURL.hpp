/*
 * ReferenceURLHandle.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_REFERENCEURLHANDLE_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_REFERENCEURLHANDLE_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Persistency/ReferenceURL.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct ReferenceURLHandle: private System::NoInstance
{
  template<typename THandleMap>
  static std::string get(const ReferenceURL &e, Params &p)
  {
    typedef boost::mpl::at<THandleMap, ErrorTests>::type ErrH;
    typedef boost::mpl::at<THandleMap, OnTerm>::type TermH;

    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, t);

    if(*t=="name")
    {
      p.callback().value( TermH::get( e.getName().get() ) )
    }
      return check( cast( e.getName().get() ), t+1 );
    if(*t=="url")
      return check( cast( e.getURL().get() ), t+1 );

    ErrH::throwInvalid(SYSTEM_SAVE_LOCATION, t);
    return false;         // we never reach here
  }
}; // struct OnReferenceURL

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
