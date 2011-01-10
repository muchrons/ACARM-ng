/*
 * Logic.cpp
 *
 */
#include "ConfigIO/Singleton.hpp"
#include "Preprocessor/Logic.hpp"
#include "Preprocessor/LogicImpl.hpp"

namespace Preprocessor
{

Logic::Logic(void):
  pimpl_( new LogicImpl( ConfigIO::Singleton::get()->preprocessorConfig() ) )
{
}

Logic::~Logic(void)
{
  // this code ensures proper destruction of pimpl.
}

bool Logic::checkAccept(const Persistency::Alert &alert) const
{
  return pimpl_->checkAccept(alert);
}

} // namespace Preprocessor
