/*
 * Strategy.cpp
 *
 */
#include "Trigger/ExtApp/Strategy.hpp"

using namespace std;
using namespace Base::Threads;

namespace Trigger
{
namespace ExtApp
{

Strategy::Strategy(const std::string &name, const Config &cfg):
  Trigger::Simple::Strategy("extapp", name, cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}

void Strategy::triggerImpl(const Node &n)
{
  // TODO
  /*
  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  io_.send( ss.str() );
  */
}

} // namespace ExtApp
} // namespace Trigger
