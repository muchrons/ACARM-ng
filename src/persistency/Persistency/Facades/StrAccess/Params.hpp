/*
 * Params.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_PARAMS_HPP_FILE

/* public header */

#include <cassert>
#include <boost/noncopyable.hpp>

#include "Persistency/Facades/StrAccess/Path.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

template<typename THandleMap, typename TResultCallback>
struct Params: private boost::noncopyable
{
  typedef THandleMap           HandleMap;
  typedef Path::const_iterator PathCIT;

  Params(const Path &path, TResultCallback &callback):
    now_(path.begin()),
    end_(path.end()),
    callback_(callback)
  {
  }

  Params(PathCIT now, PathCIT end, TResultCallback &callback):
    now_(now),
    end_(end),
    callback_(callback)
  {
  }

  const Params &operator++(void)
  {
    ++now_;
    return *this;
  }

  const std::string &get(void) const
  {
    assert(!isEnd());
    return *now_;
  }

  bool isEnd(void) const
  {
    return now_==end_;
  }

  TResultCallback &callback(void)
  {
    return callback_;
  }

private:
  PathCIT          now_;
  const PathCIT    end_;
  TResultCallback &callback_;
}; // struct Params

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
