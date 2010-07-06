/*
 * TestBase.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Input/TestBase.hpp"

namespace TestHelpers
{
namespace Input
{

typedef ::Persistency::IO::BackendFactory TmpFactory;

TestBase::TestBase(void):
  conn_( TmpFactory::create( TmpFactory::FactoryTypeName("stubs"),
                             TmpFactory::Options() ) ),
  trans_( new ::Persistency::IO::Transaction( conn_->createNewTransaction("generic_input_test_transaction") ) ),
  bf_(conn_, *trans_)
{
  trans_->commit(); // this prevents warnings in logs
  trans_.reset();   // end transaction now
}

} // namespace Input
} // namespace TestHelpers
