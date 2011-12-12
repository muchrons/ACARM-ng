/*
 * Ceil.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_CEIL_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_CEIL_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing ceil() of a given element.
 */
class Ceil: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   */
  explicit Ceil(BasePtrNN arg);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Ceil

} // namespace Formatters
} // namespace Preprocessor

#endif
