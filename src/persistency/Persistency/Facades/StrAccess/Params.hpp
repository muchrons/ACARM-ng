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
#include "Persistency/Facades/StrAccess/ResultCallback.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct Params: private boost::noncopyable
{
  typedef Path::const_iterator PathCIT;

  Params(const Path &path, ResultCallback &callback):
    path_(path),
    now_(path_.begin()),
    callback_(callback)
  {
  }

  const Params &operator++(void)
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

  ResultCallback &callback(void)
  {
    return callback_;
  }

private:
  const Path      path_;
  PathCIT         now_;
  ResultCallback &callback_;
}; // struct Params

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
