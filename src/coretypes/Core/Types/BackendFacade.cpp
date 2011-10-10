/*
 * BackendFacade.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Core/Types/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;

namespace Core
{
namespace Types
{

BackendFacade::CustomIOInterface::~CustomIOInterface(void)
{
}

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                             const Proc::CategoryName         &category,
                             const Proc::TypeName             &type,
                             const Proc::InstanceName         &name):
  log_("core.types.backendfacade"),
  category_(category),
  type_(type),
  name_(name),
  hbOwner_( getHeartbeatOwnerName() ),
  conn_(conn)
{
  // transaction is not started here yet - it will be initialized when needed
  // for the first time (not to do begin-rollback, useless traffic)
}

BackendFacade::~BackendFacade(void)
{
  // d-tor required to ensure proper destruction of forward-declared objects
}

void BackendFacade::commitChanges(void)
{
  // if no changes were introduced, just do nothing
  if( transaction_.get()==NULL )
    return;

  transaction_->commit();
  transaction_.reset();
}

void BackendFacade::performCustomIO(CustomIOInterface &ci)
{
  beginTransaction();
  ci.customAction( getConnection(), getTransaction() );
}


void BackendFacade::heartbeat(const Persistency::IO::Heartbeats::Module &m, unsigned int validFor)
{
  heartbeatImpl(m.get(), validFor);
}


void BackendFacade::heartbeat(unsigned int validFor)
{
  heartbeatImpl("self", validFor);
}


void BackendFacade::beginTransaction(void)
{
  if( transaction_.get()==NULL )    // new transaction
  {
    TransactionAPIAutoPtr api=conn_->createNewTransaction( "transaction_for_" + getName().str() );
    transaction_.reset( new Transaction(api) );
  }
  // if begin has been requested, transaction must always be valid
  transaction_->ensureIsActive();
}

Transaction &BackendFacade::getTransaction(void)
{
  assert( transaction_.get()!=NULL );
  return *transaction_;
}

Persistency::IO::ConnectionPtrNN BackendFacade::getConnection(void)
{
  return conn_;
}

Persistency::IO::DynamicConfigAutoPtr BackendFacade::createDynamicConfig(const Persistency::IO::DynamicConfig::Owner &owner)
{
  beginTransaction();
  assert( transaction_.get()!=NULL );
  return conn_->dynamicConfig(owner, *transaction_);
}

std::string BackendFacade::getHeartbeatOwnerName(void) const
{
  std::stringstream ss;
  ss<<category_.str()<<"::"<<type_.str()<<"/"<<name_.str();
  return ss.str();
}

void BackendFacade::heartbeatImpl(const char *mod, unsigned int validFor)
{
  assert(mod!=NULL);
  LOGMSG_DEBUG_S(log_)<<"sending heartbeat from external module '"<<mod<<"' with validity of "<<validFor<<"[s]";
  beginTransaction();
  Persistency::IO::HeartbeatsAutoPtr hb=getConnection()->heartbeats(hbOwner_, getTransaction());
  assert( hb.get()!=NULL );
  hb->report(mod, validFor);
  // transaction will be commited after everything's done
}

} // namespace Types
} // namespace Core
