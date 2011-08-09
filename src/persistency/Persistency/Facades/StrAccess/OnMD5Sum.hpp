/*
 * OnMD5Sum.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONMD5SUM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONMD5SUM_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Persistency/MD5Sum.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief handle processing md5sum objects.
 */
struct OnMD5Sum: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const MD5Sum &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfNotLast(SYSTEM_SAVE_LOCATION, p);
    if(p.get()!="md5sum")
      ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return MainDispatcher::process(e.get(), p);
  }
}; // struct OnMD5Sum

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
