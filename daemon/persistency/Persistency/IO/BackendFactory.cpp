#include "Persistency/IO/BackendFactory.hpp"

namespace Persistency
{
namespace IO
{

BackendFactory::FactoryPtr BackendFactory::create(void)
{
  // TODO: get name from config
  const Singleton::FactoryTypeName name="stubs";    // TODO: temporary solution
  // TODO: get options form configuration
  const Singleton::Options         options;

  return Singleton::create(name, options);
}

} // namespace IO
} // namespace Persistency
