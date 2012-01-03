/*
 * Mod.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_MOD_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_MOD_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"
#include "Preprocessor/Formatters/ExceptionDivisionByZero.hpp"
#include "Preprocessor/Formatters/ExceptionUnexpectedFloatingPoint.hpp"
#include "Preprocessor/Formatters/ExceptionUnexpectedNegativeNumber.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing module of element one by elenment two.
 */
class Mod: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param a element to be computed module from.
   *  \param b element to modulo by.
   *
   *  returns a%b;
   */
  Mod(BasePtrNN a, BasePtrNN b);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Mod

} // namespace Formatters
} // namespace Preprocessor

#endif
