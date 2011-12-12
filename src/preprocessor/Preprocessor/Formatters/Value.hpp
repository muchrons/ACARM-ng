/*
 * Value.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_VALUE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_VALUE_HPP_FILE

#include <string>

#include "Commons/SharedPtrNotNULL.hpp"
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

  /** \brief sets value to be returned on exec.
   *  \param str value to be set.
   */
  void set(const std::string &str)
  {
    LOGMSG_DEBUG_S(log_)<<"setting value to '"<<str<<"'";
    str_=str;
  }

  /** \brief sets value to be returned on exec.
   *  \param v value to be set.
   */
  void set(const double d);

private:
  virtual std::string execImpl(const Arguments &args) const;

  std::string str_;
}; // class Value


/** \brief value shared pointer.
 *
 *  this pointer is written, since it is often required to operate
 *  on Value* and pass Base* to the other calls. this wrapper makes
 *  both actions possible.
 */
typedef Commons::SharedPtrNotNULL<Value> ValuePtrNN;

} // namespace Formatters
} // namespace Preprocessor

#endif
