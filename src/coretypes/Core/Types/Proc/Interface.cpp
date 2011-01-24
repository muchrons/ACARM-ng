/*
 * Interface.cpp
 *
 */
#include "Core/Types/Proc/Interface.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{

Interface::~Interface(void)
{
}

const std::string &Interface::getType(void) const
{
  return type_;
}

const std::string &Interface::getName(void) const
{
  return name_;
}

const EntryControlList &Interface::getECL(void) const
{
  return ecl_;
}

Interface::Interface(const std::string &type, const std::string &name, const EntryControlList &ecl):
  type_(type),
  name_(name),
  ecl_(ecl)
{
}


} // namespace Proc
} // namespace Types
} // namespace Core
