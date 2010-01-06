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

const std::string &Interface::getName(void) const
{
  return name_;
}

Interface::Interface(const std::string &name):
  name_(name)
{
}


} // namespace Proc
} // namespace Types
} // namespace Core
