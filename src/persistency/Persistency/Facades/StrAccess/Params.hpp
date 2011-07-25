/*
 * Params.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE

/* public header */

#include <cassert>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>

#include "Persistency/Facades/StrAccess/Path.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/detail/Term.hpp"
#include "Persistency/Facades/StrAccess/detail/NonTerm.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

template<typename THandleMap, typename TResultCallback>
struct Params
{
  typedef THandleMap           HandleMap;
  typedef TResultCallback      ResultCallback;
  typedef Path::const_iterator PathCIT;

  struct OnUnknownType
  {
    template<typename T>
    static bool process(const T &/*e*/, Params<THandleMap,TResultCallback> &p)
    {
      handle<ErrorTests>::type::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p);
      return false; // code never reaches here
    }
  }; // struct OnUnknownType

  Params(const Path &path, ResultCallback &callback):
    path_(path),
    now_(path_.begin()),
    callback_(&callback)
  {
    assert(callback_!=NULL);
  }

  template<typename T>
  struct handle
  {
  private:
    typedef typename boost::mpl::if_c< boost::mpl::has_key<HandleMap,T>::value,
                                     // then
                                       typename boost::mpl::at<HandleMap,T>::type,
                                     // else
                                       OnUnknownType
                                     >::type OnNonTerm;

    typedef typename boost::mpl::if_c< IsTerm<T>::value,
                                     // then
                                       detail::Term,
                                     // else
                                       OnNonTerm
                                     >::type IfTerm;

  public:
    // response
    typedef IfTerm type;
  }; // struct handle

  Params &operator++(void)
  {
    assert(!isEnd());
    ++now_;
    return *this;
  }

  const Path &path(void) const
  {
    return path_;
  }

  const std::string &get(void) const
  {
    assert(!isEnd());
    return *now_;
  }

  bool isEnd(void) const
  {
    return now_==path_.end();
  }

  bool hasNext(void) const
  {
    if(isEnd())
      return false;
    PathCIT tmp=now_;
    ++tmp;
    return tmp!=path_.end();
  }

  ResultCallback &callback(void)
  {
    assert(callback_!=NULL);
    return *callback_;
  }

private:
  const Path      path_;
  PathCIT         now_;
  ResultCallback *callback_;
}; // struct Params

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
