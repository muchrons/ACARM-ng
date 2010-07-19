/*
 * Null.cpp
 *
 */

#include "Logger/Appenders/Null.hpp"


namespace Logger
{
namespace Appenders
{

void Null::appendImpl(const std::string &/*str*/)
{
  // input is ignored
}

void Null::reinitImpl(void)
{
  // this call is ignored
}

const char *Null::getTypeNameImpl(void) const
{
  return getThisTypeName();
}

} // namespace Appenders
} // namespace Logger
