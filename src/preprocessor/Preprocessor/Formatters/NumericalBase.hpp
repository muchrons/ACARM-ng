/*
 * NumericalBase.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_NUMERICALBASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_NUMERICALBASE_HPP_FILE

#include "Preprocessor/Formatters/MixedBase.hpp"
#include "Preprocessor/Formatters/ExceptionNotANumber.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief base class for all formatter operating on numerical values only.
 */
class NumericalBase: public MixedBase
{
protected:
  /** \brief create object instance of a given action.
   *  \param name name of this action (used for logging).
   *  \param fmts formatters to take source data from.
   */
  NumericalBase(const std::string &name, const Formatters &fmts);

private:
  virtual void onNonNumeric(const std::string &v) const;
  virtual std::string execConverted(const Arguments &args) const;
}; // class NumericalBase

} // namespace Formatters
} // namespace Preprocessor

#endif
