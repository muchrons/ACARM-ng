/*
 * ErrorHandle.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORHANDLE_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORHANDLE_HPP_FILE

/* public header */

#include <cassert>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"
#include "Persistency/Facades/StrAccess/ExceptionInvalidPath.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct ErrorHandle: private System::NoInstance
{
  static void throwIfEnd(const Params &p)
  {
    if(p.isEnd())
      throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, p.path().get(), "<END>", "unexpected end of path");
  }

  static void throwIfNotEnd(const Params &p)
  {
    if(!p.isEnd())
      throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, p.path().get(), p.get(), "unexpected tokens after full path");
  }

  static void throwOnInvalidPath(const Params &p)
  {
    if(!p.isEnd())
      throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, p.path().get(), p.get(), "unexpected token in path");
  }
}; // struct ErrorHandle

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
