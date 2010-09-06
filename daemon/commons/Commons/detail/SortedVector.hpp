/*
 * SortedVector.hpp
 *
 */
#ifndef INCLUDE_COMMONS_DETAIL_SORTEDVECTOR_HPP_FILE
#define INCLUDE_COMMONS_DETAIL_SORTEDVECTOR_HPP_FILE

/* public header */

#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>

namespace Commons
{
namespace detail
{

/** \brief helper for sorting vectors
 */
template<typename TCol>
class SortedVector
{
public:
  typedef typename TCol::value_type           TValue;
private:
  typedef std::vector<TValue>                 Collection;
public:
  typedef typename Collection::const_iterator const_iterator;

  explicit SortedVector(const TCol &c)
  {
    c_.reserve( c.size() );
    std::copy( c.begin(), c.end(), std::insert_iterator<Collection>( c_, c_.begin() ) );
    assert( c.size()==c_.size() );
    std::sort( c_.begin(), c_.end() );
  }

  const Collection &get(void) const
  {
    return c_;
  }

private:
  Collection c_;
}; // class SortedVector


} // namespace detail
} // namespace Commons

#endif
