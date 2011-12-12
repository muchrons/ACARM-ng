/*
 * NumericalBase.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/NumericalBase.hpp"


namespace Preprocessor
{
namespace Formatters
{

NumericalBase::NumericalBase(const std::string &name, const Formatters &fmts):
  MixedBase(name, fmts)
{
}


void NumericalBase::onNonNumeric(const std::string &v) const
{
  LOGMSG_ERROR_S(log_)<<"unexpected non-number spotted: '"<<v<<"'";
  throw ExceptionNotANumber(SYSTEM_SAVE_LOCATION, v);
}


std::string NumericalBase::execConverted(const Arguments &/*args*/) const
{
  assert(!"this should be never called");
  throw std::logic_error("this should be never called");
  return "this code is never reached";
}

} // namespace Formatters
} // namespace Preprocessor
