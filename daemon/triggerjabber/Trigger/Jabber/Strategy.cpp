/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/MessageIO.hpp"

using namespace std;
using namespace Base::Threads;

namespace Trigger
{
namespace Jabber
{

Strategy::Strategy(const std::string &name, const Config &cfg):
  Trigger::Simple::Strategy("jabber", name, cfg.getThresholdConfig() ),
  io_(cfg)
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  io_.send( ss.str() );
}

} // namespace Jabber
} // namespace Trigger
