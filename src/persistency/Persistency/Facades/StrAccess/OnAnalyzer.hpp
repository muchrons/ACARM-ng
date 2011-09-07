/*
 * OnAnalyzer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONANALYZER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONANALYZER_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief handle processing analyzer objects.
 */
struct OnAnalyzer: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename TParams>
  static bool process(const Analyzer &e, TParams p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "analyzer");

    ++p;

    if(p.get()=="id")
      return MainDispatcher::process(e.getID().get(), p);
    if(p.get()=="name")
      return MainDispatcher::process(e.getName().get(), p);
    if(p.get()=="version")
      return MainDispatcher::process(e.getVersion().get(), p);
    if(p.get()=="os")
      return MainDispatcher::process(e.getOperatingSystem().get(), p);
    if(p.get()=="ip")
      return MainDispatcher::process(e.getIP(), p);

    ErrH::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
    return false;
  }
}; // struct OnAnalyzer

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
