/*
 * Max.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_MAX_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_MAX_HPP_FILE

#include "Preprocessor/Formatters/MixedBase.hpp"
#include "Preprocessor/Formatters/ExceptionInvalidNumberOfArguments.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class representing max() operation on multiple arguments.
 */
class Max: public MixedBase
{
public:
  /** \brief create object instance of a given action.
   *  \param fmts formatters to take source data from (must be at least 2).
   */
  explicit Max(const Formatters &fmts);

private:
  virtual void onNonNumeric(const std::string &v) const;
  virtual double execConverted(const NumericalArguments &args) const;
  virtual std::string execConverted(const Arguments &args) const;
}; // class MixedBase

} // namespace Formatters
} // namespace Preprocessor

#endif
