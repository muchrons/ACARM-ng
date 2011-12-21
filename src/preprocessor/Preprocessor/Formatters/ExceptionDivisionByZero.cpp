/*
 * ExceptionDivisionByZero.cpp
 *
 */
#include "Preprocessor/Formatters/ExceptionDivisionByZero.hpp"


namespace Preprocessor
{
namespace Formatters
{

ExceptionDivisionByZero::ExceptionDivisionByZero(const Location &where):
  Exception(where, "division by zero requested")
{
}

} // namespace Formatters
} // namespace Preprocessor
