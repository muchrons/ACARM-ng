/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/Python/Strategy.hpp"

using namespace std;
using namespace Core::Types::Proc;

namespace Trigger
{
namespace Python
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Trigger::Simple::Strategy(TypeName("python"), name, cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // TODO: call python here
}

} // namespace Python
} // namespace Trigger
