/*
 * NumericalBase.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_NUMERICALBASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_NUMERICALBASE_HPP_FILE

#include "Preprocessor/Formatters/Base.hpp"
#include "Preprocessor/Formatters/ExceptionNotANumber.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief base class for all formatters.
 */
class NumericalBase: public Base
{
public:
  /** \brief arguments vector, to pass to the implementaion. */
  typedef std::vector<double> NumericalArguments;

protected:
  /** \brief create object instance of a given action.
   *  \param name name of this action (used for logging).
   *  \param fmts formatters to take source data from.
   */
  NumericalBase(const std::string &name, const Formatters &fmts);

private:
  virtual std::string execImpl(const Arguments &args) const;
  virtual double execConverted(const NumericalArguments &args) const = 0;
}; // class NumericalBase

} // namespace Formatters
} // namespace Preprocessor

#endif
