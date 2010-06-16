/*
 * Strategy.cpp
 *
 */
//#include <sstream>
#include <cassert>

//#include "Algo/countCorrelatedAlerts.hpp"
//#include "Algo/computeSeverity.hpp"
#include "Trigger/File/Strategy.hpp"

using namespace std;

namespace Trigger
{
namespace File
{

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("file", cfg.getThresholdConfig() ),
  outdir_( cfg.getOutputDirectory() )
{
}

void Strategy::trigger(const Node &n)
{
  // TODO:
  /*
  stringstream ss;
  ss << "reporting triggered for meta-alert '"
     << n->getMetaAlert().getName().get()
     << "' (" << Algo::countCorrelatedAlerts(n)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(n) << ")";
  Connection    conn(ggCfg_);
  MessageSender ms(conn);
  ms.send(receiver_, ss.str() );
  */
}

} // namespace File
} // namespace Trigger
