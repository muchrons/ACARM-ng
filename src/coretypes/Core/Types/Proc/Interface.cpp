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

const TypeName &Interface::getType(void) const
{
  return type_;
}

const InstanceName &Interface::getName(void) const
{
  return name_;
}

const EntryControlList &Interface::getECL(void) const
{
  return ecl_;
}

Interface::Interface(const TypeName &type, const InstanceName &name, const EntryControlList &ecl):
  type_(type),
  name_(name),
  ecl_(ecl)
{
}


} // namespace Proc
} // namespace Types
} // namespace Core
