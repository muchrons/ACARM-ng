/*
 * Abs.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_ABS_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_ABS_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing absolute value of a given number.
 */
class Abs: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param arg argument to compute absolute value from.
   */
  explicit Abs(BasePtrNN arg);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Abs

} // namespace Formatters
} // namespace Preprocessor

#endif
