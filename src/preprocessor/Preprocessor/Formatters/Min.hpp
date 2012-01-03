/*
 * Min.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_MIN_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_MIN_HPP_FILE

#include "Preprocessor/Formatters/MixedBase.hpp"
#include "Preprocessor/Formatters/ExceptionInvalidNumberOfArguments.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class representing min() operation on multiple arguments.
 */
class Min: public MixedBase
{
public:
  /** \brief create object instance of a given action.
   *  \param fmts formatters to take source data from (must be at least 2).
   */
  explicit Min(const Formatters &fmts);

private:
  virtual void onNonNumeric(const std::string &v) const;
  virtual double execConverted(const NumericalArguments &args) const;
  virtual std::string execConverted(const Arguments &args) const;
}; // class Min

} // namespace Formatters
} // namespace Preprocessor

#endif
