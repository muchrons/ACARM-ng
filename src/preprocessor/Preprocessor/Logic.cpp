/*
 * Logic.cpp
 *
 */
#include "Preprocessor/Logic.hpp"
#include "Preprocessor/LogicImpl.hpp"

namespace Preprocessor
{

Logic::Logic(const ConfigIO::Preprocessor::Config &cfg):
  pimpl_( new LogicImpl(cfg) )
{
}

Logic::~Logic(void)
{
  // this code ensures proper destruction of pimpl.
}

bool Logic::checkAccept(const Persistency::ConstGraphNodePtrNN &node) const
{
  return pimpl_->checkAccept(node);
}

} // namespace Preprocessor
