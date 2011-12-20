/*
 * Round.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_ROUND_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_ROUND_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing rounding of a given element.
 */
class Round: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param arg argument to be rounded.
   */
  explicit Round(BasePtrNN arg);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Round

} // namespace Formatters
} // namespace Preprocessor

#endif
