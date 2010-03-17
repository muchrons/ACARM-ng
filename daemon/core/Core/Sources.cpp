/*
 * Sources.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/Sources.hpp"

using namespace Persistency;

namespace Core
{

Sources::Sources(void):
  conn_( IO::create() ),
  inputs_( Input::create(queue_) )
{
}

Persistency::GraphNodePtrNN Sources::read(void)
{
  AlertPtrNN      alert=readAlert();
  IO::Transaction t( conn_->createNewTransaction("core_save_graphnode") );
  GraphNodePtrNN  leaf( new GraphNode(alert, conn_, t) );
  return leaf;
}

Persistency::AlertPtrNN Sources::readAlert(void)
{
  return queue_.pop();
}

} // namespace Core
