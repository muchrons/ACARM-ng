/*
 * LimitedNULLString.hpp
 *
 */
#ifndef INCLUDE_COMMONS_LIMITEDNULLSTRING_HPP_FILE
#define INCLUDE_COMMONS_LIMITEDNULLSTRING_HPP_FILE

/* public header */

#include <boost/operators.hpp>
#include <cassert>

#include "Base/ViaPointer.hpp"
#include "Commons/LimitedString.hpp"

namespace Commons
{

/** \brief class holding string of a given maximum length or NULL.
 */
template<uint16_t N>
class LimitedNULLString: public boost::operators< LimitedNULLString<N> >
{
public:
  /** \brief export size to be later referencable. */
  enum { size=N };

  /** \brief creates empty object string.
   */
  LimitedNULLString(void):
    str_(),
    ptr_( str_.get() )
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
  }
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
    return Base::ViaPointer::equal(ptr_, other.ptr_);
  }

private:
  LimitedString<N>  str_;
  const char       *ptr_;
}; // class LimitedString

} // namespace Commons

#endif
