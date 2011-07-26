/*
 * ErrorThrower.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORTHROWER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORTHROWER_HPP_FILE

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

/** \brief handle throwing exception on different errors.
 */
struct ErrorThrower: private System::NoInstance
{
  /** \brief throws exception if there is no next element.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwIfLast(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    if(!p.hasNext())
      throw ExceptionInvalidPath(where, p.path().get(), p.get(), "unexpected end of path");
  }

  /** \brief throws exception if there is next element available.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwIfNotLast(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    if(p.hasNext())
      throw ExceptionInvalidPath(where, p.path().get(), p.get(), "unexpected tokens after full path");
  }

  /** \brief throws exception indicating invalid token in path.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwOnInvalidPath(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    throw ExceptionInvalidPath(where, p.path().get(), p.get(), "unexpected token in path");
  }

  /** \brief throws exception indicating invalid index (out of bound).
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwOnInvalidIndex(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    throw ExceptionInvalidPath(where, p.path().get(), p.get(), "index out of bound");
  }

  /** \brief throws exception if there current element point to the end of path.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TParams>
  static void throwIfEnd(const ExceptionInvalidPath::Location &where, const TParams &p)
  {
    if(p.isEnd())
      throw ExceptionInvalidPath(where, p.path().get(), "<END>", "invalid request");
  }
}; // struct ErrorThrower

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
