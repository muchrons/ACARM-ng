/*
 * LimitedString.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_LIMITEDSTRING_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_LIMITEDSTRING_HPP_FILE

/* public header */

#include <string>
#include <cstring>
#include <cassert>
#include <boost/static_assert.hpp>

#include "Persistency/Exception.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"


namespace Persistency
{
namespace detail
{

/** \brief thrown when input string is too long.
 */
class ExceptionStringTooLong: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place wxception has been thrown.
   *  \param str   string that has been found too long.
   */
  ExceptionStringTooLong(const char *where, const char *str):
    Exception(where, std::string("too long string: ") + ensureString(str) )
  {
  }
}; // class ExceptionStringTooLong


/** \brief class holding string of a given maximum length.
 */
template<uint16_t N>
class LimitedString
{
public:
  /** \brief creates object from a given string.
   *  \param str string to create object from.
   *  \note this c-tor is not explicit to allow easier argument passing.
   */
  LimitedString(const char *str)
  {
    // sanity check
    if(str==NULL)
      throw ExceptionNULLParameter("LimitedString::LimitedString()", "str");

    // check length
    const size_t len=strlen(str);
    if(len>N)
      throw ExceptionStringTooLong("LimitedString::LimitedString()", str);

    // save data
    assert(len+1<=sizeof(str_));
    memcpy(str_, str, len+1);
  }

  /** \brief gets pointer to data.
   *  \return string held inside.
   */
  const char *get(void) const
  {
    assert( strlen(str_)<=N );
    return str_;
  }

private:
  BOOST_STATIC_ASSERT(N<=512 && "string is unreasonably large");

  char str_[N+1];
}; // class LimitedString

} // namespace detail
} // namespace Persistency

#endif
