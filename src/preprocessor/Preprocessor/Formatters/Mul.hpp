/*
 * Mul.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_MUL_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_MUL_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"
#include "Preprocessor/Formatters/ExceptionInvalidNumberOfArguments.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing multiplicity of a given elements.
 */
class Mul: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param args arguments to be multiplied. minumum 2 arguments are required.
   */
  explicit Mul(Formatters args);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Mul

} // namespace Formatters
} // namespace Preprocessor

#endif
