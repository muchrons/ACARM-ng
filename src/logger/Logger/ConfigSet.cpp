/*
 * ConfigSet.cpp
 *
 */
#include <cassert>

#include "Logger/detail/ConfigSetSingleton.hpp"
#include "Logger/ConfigSet.hpp"

namespace Logger
{

NodeConfPtr ConfigSet::getConfig(const NodeName &nn)
{
  detail::ConfigSetImpl *ptr=detail::ConfigSetSingleton::get();
  assert(ptr!=NULL);
  return ptr->getNodeConfiguration(nn);
}

} // namespace Logger
