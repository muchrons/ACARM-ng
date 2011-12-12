/*
 * MixedBase.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_MIXEDBASE_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_MIXEDBASE_HPP_FILE

#include "Preprocessor/Formatters/Base.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief base class for all formatters that handle numerical values or both.
 */
class MixedBase: public Base
{
public:
  /** \brief arguments vector, to pass to the implementaion. */
  typedef std::vector<double> NumericalArguments;

protected:
  /** \brief create object instance of a given action.
   *  \param name name of this action (used for logging).
   *  \param fmts formatters to take source data from.
   */
  MixedBase(const std::string &name, const Formatters &fmts);

private:
  virtual std::string execImpl(const Arguments &args) const;

  /** \brief callback called when non-numeric value is spotted within arguments list.
   *  \param v non-numeric value.
   */
  virtual void onNonNumeric(const std::string &v) const = 0;
  /** \brief callback called when all arguments are numerical.
   *  \param args numerical arguments list.
   *  \return computed value to be returned for further processing.
   */
  virtual double execConverted(const NumericalArguments &args) const = 0;
  /** \brief callback called when not all arguments are numerical.
   *  \param args arguments list.
   *  \return computed value to be returned for further processing.
   */
  virtual std::string execConverted(const Arguments &args) const = 0;
}; // class MixedBase

} // namespace Formatters
} // namespace Preprocessor

#endif
