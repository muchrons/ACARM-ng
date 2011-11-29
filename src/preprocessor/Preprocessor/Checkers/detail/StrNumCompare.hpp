/*
 * StrNumCompare.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_STRNUMCOMPARE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_STRNUMCOMPARE_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"
#include "Preprocessor/Checkers/detail/NumberWrapper.hpp"

namespace Preprocessor
{
namespace Checkers
{
namespace detail
{

/** \brief class checking two strings with less-than order.
 */
template<typename Cmp>
class StrNumCompare: public Mode
{
public:
  /** \brief check if given string is less than string given in c-tor.
   *  \param str string to check.
   *  \return true if string is less than given one, false otherwise.
   *  \note if both strings are numbers, comparison is not alphabetical, but numerical.
   */
  virtual bool check(const std::string &str) const
  {
    const detail::NumberWrapper p(str);
    // numerical comparison is possible?
    if( num_.isNumber() && p.isNumber() )
      return cmp_( p.get(), num_.get() );
    // if not, fallback to lexicographical
    return cmp_(str, str_);
  }

protected:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  StrNumCompare(const char *modeName, const std::string &str):
    Mode(modeName),
    str_(str),
    num_(str_),
    cmp_()
  {
  }

private:
  const std::string   str_;
  const NumberWrapper num_;
  const Cmp           cmp_;
}; // class StrNumCompare

} // namespace detail
} // namespace Checkers
} // namespace Preprocessor

#endif
