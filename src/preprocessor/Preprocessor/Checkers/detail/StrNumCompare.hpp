/*
 * StrNumCompare.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_STRNUMCOMPARE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_STRNUMCOMPARE_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Commons/NumberWrapper.hpp"
#include "Preprocessor/Checkers/Mode.hpp"

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
    const bool  ret=compareImpl(str);
    const char *msg=(ret?"true":"false");
    LOGMSG_DEBUG_S(log_)<<"result of comparison: '"<<str<<"' cmp-operator '"<<str_<<"'; is "<<msg;
    return ret;
  }

protected:
  /** \brief create checker.
   *  \param modeName name of the comparison type.
   *  \param str      string to be compared on demand.
   */
  StrNumCompare(const std::string &modeName, const std::string &str):
    Mode(modeName),
    log_("preprocessor.checkers.detail.strnumcompare"),
    str_(str),
    num_(str_),
    cmp_()
  {
  }

private:
  bool compareImpl(const std::string &str) const
  {
    const Commons::NumberWrapper p(str);
    // numerical comparison is possible?
    if( num_.isNumber() && p.isNumber() )
    {
      LOGMSG_DEBUG(log_, "performing numerical comparison");
      return cmp_( p.get(), num_.get() );
    }
    // if not, fallback to lexicographical
    LOGMSG_DEBUG(log_, "performing lexicographical comparison");
    return cmp_(str, str_);
  }

  const Logger::Node           log_;
  const std::string            str_;
  const Commons::NumberWrapper num_;
  const Cmp                    cmp_;
}; // class StrNumCompare

} // namespace detail
} // namespace Checkers
} // namespace Preprocessor

#endif
