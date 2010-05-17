/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/GG/Strategy.hpp"

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

void Strategy::trigger(const Node &n)
{
  // TODO
}

} // namespace GG
} // namespace Trigger
