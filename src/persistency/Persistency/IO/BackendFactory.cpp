#include "Persistency/IO/BackendFactory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace ConfigIO;

namespace Persistency
{
namespace IO
{

BackendFactory::FactoryPtr create(void)
{
  const PersistencyConfig               &cfg    =Singleton::get()->persistencyConfig();
  const BackendFactory::FactoryTypeName &name   =cfg.getType();
  const BackendFactory::Options         &options=cfg.getOptions();
  return BackendFactory::create(name, options);
}

} // namespace IO
} // namespace Persistency
