/*
 * Source.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/Source.hpp"

using namespace Persistency;

namespace Core
{

Source::Source(void):
  conn_( IO::create() ),
  inputs_( Input::create(queue_) )
{
}

Persistency::GraphNodePtrNN Source::read(void)
{
  AlertPtrNN      alert=readAlert();
  IO::Transaction t( conn_->createNewTransaction("core_save_graphnode") );
  GraphNodePtrNN  leaf( new GraphNode(alert, conn_, t) );
  return leaf;
}

Persistency::AlertPtrNN Source::readAlert(void)
{
  return queue_.pop();
}

} // namespace Core
