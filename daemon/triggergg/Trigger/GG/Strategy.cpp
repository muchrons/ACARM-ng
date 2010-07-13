/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/Compose/Summary.hpp"
#include "Trigger/GG/Strategy.hpp"
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
  // prepare connection
  ConnectionAutoPtr conn=conn_; // take ownership
  assert( conn_.get()==NULL );
  if( conn.get()==NULL )        // create connection, if not available
    conn.reset( new Connection(ggCfg_) );
  assert( conn.get()!=NULL );

  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  // send message
  assert( conn.get()!=NULL );
  MessageSender ms( *conn.get() );
  ms.send(receiver_, ss.str() );

  // save opened connection for later usage
  conn_=conn;
  assert( conn.get() ==NULL );
  assert( conn_.get()!=NULL );
}

} // namespace GG
} // namespace Trigger
