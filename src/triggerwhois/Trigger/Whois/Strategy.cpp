/*
 * Strategy.cpp
 *
 */

#include "Trigger/Whois/Strategy.hpp"

using namespace Core::Types::Proc;

namespace Trigger
{
namespace Whois
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Trigger::Simple::Strategy(TypeName("whois"), name, cfg.getThresholdConfig() )
{
}

void Strategy::triggerImpl(BackendFacade &/*bf*/, const ConstNode &/*n*/)
{
  // TODO
}

} // namespace Whois
} // namespace Trigger
