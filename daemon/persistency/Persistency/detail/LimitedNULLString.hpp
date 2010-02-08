/*
 * LimitedNULLString.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_LIMITEDNULLSTRING_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_LIMITEDNULLSTRING_HPP_FILE

/* public header */

#include <boost/operators.hpp>
#include <cassert>

#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
namespace detail
{

/** \brief class holding string of a given maximum length or NULL.
 */
template<uint16_t N>
class LimitedNULLString: public boost::operators< LimitedNULLString<N> >
{
public:
  /** \brief creates object from a given string.
   *  \param str string to create object from.
   *  \note this c-tor is not explicit to allow easier argument passing.
   */
  LimitedNULLString(const char *str):
    str_( (str!=NULL)?str       :""   ),
    ptr_( (str!=NULL)?str_.get():NULL )
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
  }
  /** \brief creates object from a given string.
   *  \param str string to create object from.
   *  \note this c-tor is not explicit to allow easier argument passing.
   */
  LimitedNULLString(const std::string &str):
    str_(str),
    ptr_( str_.get() )
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
  }

  /** \brief gets pointer to data.
   *  \return string held inside.
   */
  const char *get(void) const
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
    return ptr_;
  }

  /** \brief gets char from a given position.
   *  \param p position to get char from.
   *  \return char.
   *  \note call does NOT check ranges.
   */
  char operator[](const uint16_t p) const
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
    return str_[p];
  }

  /** \brief copy c-tor.
   *  \param o object ot copy from.
   */
  LimitedNULLString(const LimitedNULLString &o):
    str_(o.str_),
    ptr_( (o.ptr_!=NULL)?str_.get():NULL )
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
  }

  /** \brief assignment operator.
   *  \param o object to copy from.
   *  \return reterence to this object.
   */
  const LimitedNULLString<N> &operator=(const LimitedNULLString &o)
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
    if(this!=&o)
    {
      str_=o.str_;
      ptr_=(o.ptr_!=NULL)?str_.get():NULL;
    }
    assert( ptr_==NULL || str_.get()==ptr_ );
    return *this;
  }

  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const LimitedNULLString<N> &other) const
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
    // check if both are NULLs, or are equal
    if(ptr_==other.ptr_)
      return true;
    // check if only one is NULL
    if( ( ptr_==NULL && other.ptr_!=NULL ) ||
        ( ptr_!=NULL && other.ptr_==NULL )    )
      return false;
    // compare strings
    assert(ptr_!=NULL);
    assert(other.ptr_!=NULL);
    assert( ( str_==other.str_ ) == ( strcmp(ptr_, other.ptr_)==0 ) );
    return strcmp(ptr_, other.ptr_)==0;
  }

private:
  LimitedString<N>  str_;
  const char       *ptr_;
}; // class LimitedString

} // namespace detail
} // namespace Persistency

#endif
