/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/Compose/Summary.hpp"
#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/MessageSender.hpp"

using namespace std;

namespace Trigger
{
namespace GG
{

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("gg", cfg.getThresholdConfig() ),
  ggCfg_( cfg.getAccountConfig() ),
  receiver_( cfg.getReceiver() )
{
}

void Strategy::triggerImpl(const Node &n)
{
  // prepare data:
  stringstream  ss;
  Compose::Summary::append(ss, n);

  // prepare connection and send:
  Connection    conn(ggCfg_);
  MessageSender ms(conn);
  ms.send(receiver_, ss.str() );
}

} // namespace GG
} // namespace Trigger
