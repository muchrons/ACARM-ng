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
  template<typename TParams>
  static void throwIfLast(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    if(!p.hasNext())
      throw ExceptionInvalidPath(where, p.path().get(), "<END>", "unexpected end of path");
  }

  template<typename TParams>
  static void throwIfNotLast(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    if(p.hasNext())
      throw ExceptionInvalidPath(where, p.path().get(), p.get(), "unexpected tokens after full path");
  }

  template<typename TParams>
  static void throwOnInvalidPath(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    throw ExceptionInvalidPath(where, p.path().get(), p.get(), "unexpected token in path");
  }
}; // struct ErrorHandle

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
