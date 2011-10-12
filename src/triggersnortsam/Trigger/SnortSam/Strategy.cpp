/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Algo/GatherIPs.hpp"
#include "Trigger/SnortSam/Strategy.hpp"

using namespace Core::Types::Proc;

namespace Trigger
{
namespace SnortSam
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg, ProtocolAutoPtr proto):
  Trigger::Simple::Strategy(TypeName("snortsam"), name, cfg.getThresholdConfig() ),
  proto_( proto.release() ),
  bf_(NULL)
{
  if(proto_.get()==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "protocol cannot be NULL");
}

BackendFacade &Strategy::getBackendFacade(void)
{
  assert(bf_!=NULL);
  return *bf_;
}

void Strategy::triggerImpl(BackendFacade &bf, const ConstNode &n)
{
  bf_=&bf;
  assert( &getBackendFacade()==&bf && "helper not saved for further usage" );

  // collect all required elements
  const Algo::GatherIPs         ips(n);
  const Algo::GatherIPs::IPSet &from=ips.getSourceIPs();
  const Algo::GatherIPs::IPSet &to  =ips.getTargetIPs();

  // block all trafic between given instances
  assert(proto_.get()!=NULL);
  for(Algo::GatherIPs::IPSet::const_iterator itF=from.begin(); itF!=from.end(); ++itF)
    for(Algo::GatherIPs::IPSet::const_iterator itT=to.begin(); itT!=to.end(); ++itT)
      proto_->block(itF->first, itT->first);

  bf_=NULL;
}

} // namespace SnortSam
} // namespace Trigger
