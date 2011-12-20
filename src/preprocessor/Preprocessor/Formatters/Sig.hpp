/*
 * Sig.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_SIG_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_SIG_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing signum function of a given number.
 */
class Sig: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param arg argument to be passed through a signum function.
   */
  explicit Sig(BasePtrNN arg);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Sig

} // namespace Formatters
} // namespace Preprocessor

#endif
