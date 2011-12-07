/*
 * Value.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_VALUE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_VALUE_HPP_FILE

#include "Preprocessor/Formatters/Base.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class representing "value()" function, that always returns an
 *         argument passwd to the whole expression.
 */
class Value: public Base
{
public:
  /** \brief create object instance of a given action.
   */
  Value(void);

  /** \brief sets value to be returned to a given one.
   *  \param str value to be set.
   */
  void set(const std::string &str)
  {
    LOGMSG_DEBUG_S(log_)<<"setting value to '"<<str<<"'";
    str_=str;
  }

private:
  virtual std::string execImpl(const Arguments &args) const;

  std::string str_;
}; // class Value

} // namespace Formatters
} // namespace Preprocessor

#endif
