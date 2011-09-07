/*
 * ErrorThrower.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORTHROWER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ERRORTHROWER_HPP_FILE

/* public header */

#include <boost/mpl/at.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/detail/isIndex.hpp"

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
  /** \brief throws exception if element's name is invalid and is not and index.
   *  \param where location where condition is checked.
   *  \param name  expected name in the path.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnInvalidName(const TLocation &where, const TParams &p, const std::string &name)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    if( p.get()!=name && !detail::isIndex(p.get()) )
      throw ExceptionType(where, p.path().get(), p.get(), "invalid name/index in path");
  }

  /** \brief throws exception if there is no next element.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnLast(const TLocation &where, const TParams &p)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    if(!p.hasNext())
      throw ExceptionType(where, p.path().get(), p.get(), "unexpected end of path");
  }

  /** \brief throws exception if there is next element available.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnNotLast(const TLocation &where, const TParams &p)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    if(p.hasNext())
      throw ExceptionType(where, p.path().get(), p.get(), "unexpected tokens after full path");
  }

  /** \brief throws exception indicating invalid token in path.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnInvalidPath(const TLocation &where, const TParams &p)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    throw ExceptionType(where, p.path().get(), p.get(), "unexpected token in path");
  }

  /** \brief throws exception indicating invalid index (out of bound).
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnInvalidIndex(const TLocation &where, const TParams &p)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    throw ExceptionType(where, p.path().get(), p.get(), "index out of bound");
  }

  /** \brief throws exception if there current element point to the end of path.
   *  \param where location where condition is checked.
   *  \param p     current params value.
   */
  template<typename TLocation, typename TParams>
  static void throwOnEnd(const TLocation &where, const TParams &p)
  {
    typedef typename boost::mpl::at<typename TParams::HandleMap, InvalidPathExceptionType>::type ExceptionType;
    if(p.isEnd())
      throw ExceptionType(where, p.path().get(), "<END>", "invalid request");
  }
}; // struct ErrorThrower

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
