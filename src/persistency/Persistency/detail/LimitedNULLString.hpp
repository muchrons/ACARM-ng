/*
 * LimitedNULLString.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_LIMITEDNULLSTRING_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_LIMITEDNULLSTRING_HPP_FILE

/* public header */

#include "Persistency/detail/LimitedString.hpp"

namespace Persistency
{
namespace detail
{

/** \brief class holding string of a given maximum length or NULL.
 */
template<uint16_t N>
class LimitedNULLString
{
public:
  /** \brief creates object from a given string.
   *  \param str string to create object from.
   *  \note this c-tor is not explicit to allow easier argument passing.
   */
  LimitedNULLString(const char *str):
    str_( (str!=NULL)?str       :"" ),
    ptr_( (str!=NULL)?str_.get():NULL)
  {
  }

  /** \brief gets pointer to data.
   *  \return string held inside.
   */
  const char *get(void) const
  {
    assert( ptr_==NULL || str_.get()==ptr_ );
    return ptr_;
  }

private:
  LimitedString<N>  str_;
  const char       *ptr_;
}; // class LimitedString

} // namespace detail
} // namespace Persistency

#endif
