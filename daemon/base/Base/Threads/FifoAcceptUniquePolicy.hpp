/*
 * FifoAcceptUniquePolicy.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_FIFOACCEPTUNIQUEPOLICY_HPP_FILE
#define INCLUDE_BASE_THREADS_FIFOACCEPTUNIQUEPOLICY_HPP_FILE

/* public header */

#include <algorithm>

namespace Base
{
namespace Threads
{

/** \brief policy accepting elements that are not yet in queue
 */
struct FifoAcceptUniquePolicy
{
  /** \brief policy for accepting (enqueuing) unique elements in fifo queue.
   *  \param c collection to check against.
   *  \param e element to be checked.
   *  \return return true if ... TODO: comment this
   */
  template<typename TCollection, typename TElement>
  inline bool operator()(const TCollection &c, const TElement &e) const
  {
    return std::find(c.begin(), c.end(), e)==c.end();
  }
}; // struct FifoAcceptUniquePolicy

} // namespace Threads
} // namespace Base

#endif
