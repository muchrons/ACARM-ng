/*
 * Abs.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_ABS_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_ABS_HPP_FILE

#include "Preprocessor/Formatters/Base.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing absolute value of a given number.
 */
class Abs: public Base
{
public:
  /** \brief create object instance of a given action.
   */
  explicit Abs(BasePtrNN arg);

private:
  virtual std::string execImpl(const Arguments &args) const;
}; // class Abs

} // namespace Formatters
} // namespace Preprocessor

#endif
