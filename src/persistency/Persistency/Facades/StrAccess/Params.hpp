/*
 * Params.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE

/* public header */

#include <cassert>
#include <boost/mpl/at.hpp>

#include "Persistency/Facades/StrAccess/Path.hpp"

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
    typedef typename boost::mpl::at<HandleMap,T>::type type;
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
    assert(!isEnd());
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
