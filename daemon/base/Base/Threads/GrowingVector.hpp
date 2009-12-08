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

/** \brief template container for multi-threaded usage.
 *
 * this is growing-only container that can hold any number of elements inside.
 * access to its elements is thread-safe, though elements must be guarded
 * separetely, if needed.
 *
 * iterators are guaranteed to be always valid (even after container resize
 * or event removing original reference to it), though it may happen that
 * iterators will be pointing to previous content of collection, if
 * other thread adds any element to it.
 * if elements of the collection are shared, such scenariu requires them
 * to be thread safe.
 */
template<typename T>
class GrowingVector
{
private:
  typedef std::vector<T> V;
public:
  /** \brief non-const iterator on elements.
   */
  typedef typename V::iterator       iterator;
  /** \brief const iterator on elements.
   */
  typedef typename V::const_iterator const_iterator;

  /** \brief gets begin iterator to collection.
   *  \return non-const being interator.
   */
  iterator begin(void)
  {
    return vec_.begin();
  }
  /** \brief gets end iterator to collection.
   *  \return non-const end interator.
   */
  iterator end(void)
  {
    return vec_.end();
  }

  /** \brief gets begin iterator to collection.
   *  \return const being interator.
   */
  const_iterator begin(void) const
  {
    return vec_.begin();
  }
  /** \brief gets end iterator to collection.
   *  \return const end interator.
   */
  const_iterator end(void) const
  {
    return vec_.end();
  }

  /** \brief adds new element to collection.
   *  \param t element to be added.
   */
  void push(const T &t)
  {
    vec_.push_back(t);
  }

  /** \brief random-access operator.
   *  \param pos position to take element from.
   *  \return reference to the element on a given posiotion.
   *  \note this is thread safe, since this collection may only grow in time.
   */
  T &operator[](unsigned int pos)
  {
    return vec_.at(pos);
  }
  /** \brief random-access operator - const version.
   *  \param pos position to take element from.
   *  \return reference to the element on a given posiotion.
   *  \note this is thread safe, since this collection may only grow in time.
   */
  const T &operator[](unsigned int pos) const
  {
    return vec_.at(pos);
  }

  /** \brief gets collection's total size.
   *  \return size of collection, in number of elements.
   *  \note after this call exits it is guaranteed that collection has at least
   *        retunred number of elements.
   */
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
