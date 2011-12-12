/*
 * Add.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_ADD_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_ADD_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"
#include "Preprocessor/Formatters/ExceptionInvalidNumberOfArguments.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing sum of given elements.
 */
class Add: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   */
  explicit Add(Formatters args);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Add

} // namespace Formatters
} // namespace Preprocessor

#endif
