/*
 * GrowingVector.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_GWORINGVECTOR_HPP_FILE
#define INCLUDE_BASE_THREADS_GWORINGVECTOR_HPP_FILE

/* public header */

#include <vector>

namespace Base
{
namespace Threads
{

//
// TODO: THIS CONTAINER IS INTENDED TO BE THREAD SAFE, BUT NOW IT IS JUST A
//       QUICK STUB FOR THE API WHICH DOES NOT GUARANTEE ANY THREAD-SAFETY
//       AT ALL! IMPLEMENT IT BEFORE USING IN MULTI-THREADED CODE!
//

template<typename T>
class GrowingVector
{
private:
  typedef std::vector<T> V;
public:
  typedef typename V::iterator       iterator;
  typedef typename V::const_iterator const_iterator;

  iterator begin(void)
  {
    return vec_.begin();
  }
  iterator end(void)
  {
    return vec_.end();
  }

  const_iterator begin(void) const
  {
    return vec_.begin();
  }
  const_iterator end(void) const
  {
    return vec_.end();
  }

  void push(const T &t)
  {
    vec_.push_back(t);
  }

  T &operator[](unsigned int pos)
  {
    return vec_.at(pos);
  }
  const T &operator[](unsigned int pos) const
  {
    return vec_.at(pos);
  }

  unsigned int size(void) const
  {
    return vec_.size();
  }

private:
  V vec_;
}; // class GrowingVector

} // namespace Threads
} // namespace Base

#endif
