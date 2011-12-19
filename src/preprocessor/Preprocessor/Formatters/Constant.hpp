/*
 * Constant.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_CONSTANT_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_CONSTANT_HPP_FILE

#include <string>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Preprocessor/Formatters/Base.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class representing constatnt value, given in the config.
 */
class Constant: public Base
{
public:
  /** \brief create object instance of a given action.
   *  \param str constant value to be represented.
   */
  explicit Constant(const std::string &str);

private:
  virtual std::string execImpl(const Arguments &args) const;

  const std::string str_;
}; // class Constant

} // namespace Formatters
} // namespace Preprocessor

#endif
