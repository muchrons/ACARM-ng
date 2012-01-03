/*
 * Strategy.cpp
 *
 */
#include "Trigger/SnortSam/config.hpp"
#include "Trigger/SnortSam/NetTCP.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"
#include "Trigger/SnortSam/Ver14/Strategy.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

namespace
{
Strategy::ProtocolAutoPtr makeProtocol(const Config &cfg, Protocol::Callbacks &cb)
{
  std::auto_ptr<NetIO>      n(new NetTCP(cfg.getHost(), cfg.getPort(), networkTimeout));
  Strategy::ProtocolAutoPtr p(new Protocol(cfg.getWho(), cfg.getHow(), cfg.getDuration(), cfg.getKey(), n, cb));
  return p;
} // makeProtocol()
} // unnamed namespace


Strategy::Strategy(const Core::Types::Proc::InstanceName &name, Parameters params):
  SnortSam::Strategy(name, params.cfg_, makeProtocol(params.cfg_, *this)),
  idAssigner_(params.idAssigner_)
{
}

uint32_t Strategy::assignID(void)
{
  return idAssigner_->assign( getBackendFacade() );
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
