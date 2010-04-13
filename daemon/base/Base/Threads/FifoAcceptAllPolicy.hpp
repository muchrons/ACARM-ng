/*
 * FifoAcceptAllPolicy.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_FIFOACCEPTALLPOLICY_HPP_FILE
#define INCLUDE_BASE_THREADS_FIFOACCEPTALLPOLICY_HPP_FILE

/* public header */

namespace Base
{
namespace Threads
{

/** \brief policy accepting all elements to queue.
 */
struct FifoAcceptAllPolicy
{
  /** \brief policy for accepting (enqueuing) all elements in fifo queue.
   *  \param c collection to check against.
   *  \param e element to be checked.
   */
  template<typename TCollection, typename TElement>
  inline bool operator()(const TCollection &, const TElement &) const
  {
    return true;
  }
}; // struct FifoAcceptAllPolicy

} // namespace Threads
} // namespace Base

#endif
