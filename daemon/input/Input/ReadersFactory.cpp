#include "Input/ReadersFactory.hpp"

namespace Input
{

ReadersFactory::FactoryPtr create(void)
{
  // TODO: get name from config
  const ReadersFactory::FactoryTypeName name="file";   // TODO: temporary solution
  // TODO: get options form configuration
  const ReadersFactory::Options         options;

  return ReadersFactory::create(name, options);
}

} // namespace Input
