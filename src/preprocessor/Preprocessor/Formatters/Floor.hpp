/*
 * Floor.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_FLOOR_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_FLOOR_HPP_FILE

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class computing floor() of a given element.
 */
class Floor: public NumericalBase
{
public:
  /** \brief create object instance of a given action.
   *  \param arg argument to be floored.
   */
  explicit Floor(BasePtrNN arg);

private:
  virtual double execConverted(const NumericalArguments &args) const;
}; // class Floor

} // namespace Formatters
} // namespace Preprocessor

#endif
