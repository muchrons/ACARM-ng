/*
 * Div.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_DIV_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_DIV_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"
#include "Preprocessor/Formatters/ExceptionDivisionByZero.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing division of two elements.
 */
class Div: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param a element to be divided.
   *  \param b element to devide by.
   *
   *  returns a/b;
   */
  Div(BasePtrNN a, BasePtrNN b);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Div

} // namespace Formatters
} // namespace Preprocessor

#endif
