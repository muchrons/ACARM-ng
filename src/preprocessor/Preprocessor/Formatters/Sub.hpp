/*
 * Sub.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_SUB_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_SUB_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing subbstraction of two elements.
 */
class Sub: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param a element to be substracted from.
   *  \param b element to substract.
   *
   *  returns a-b;
   */
  Sub(BasePtrNN a, BasePtrNN b);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Sub

} // namespace Formatters
} // namespace Preprocessor

#endif
