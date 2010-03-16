#include "Input/Factory.hpp"

namespace Input
{

Factory::FactoryPtr create(void)
{
  // TODO: get name from config
  const Factory::FactoryTypeName name="file";   // TODO: temporary solution
  // TODO: get options form configuration
  const Factory::Options         options;

  return Factory::create(name, options);
}

} // namespace Input
