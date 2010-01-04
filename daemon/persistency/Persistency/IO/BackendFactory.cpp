#include "Persistency/IO/BackendFactory.hpp"

namespace Persistency
{
namespace IO
{

BackendFactory::FactoryPtr create(void)
{
  // TODO: get name from config
  const BackendFactory::FactoryTypeName name="stubs";   // TODO: temporary solution
  // TODO: get options form configuration
  const BackendFactory::Options         options;

  return BackendFactory::create(name, options);
}

} // namespace IO
} // namespace Persistency
