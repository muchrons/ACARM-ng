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
  conn_( TmpFactory::create( TmpFactory::FactoryTypeName("stubs"), TmpFactory::Options() ) ),
  bf_(conn_)
{
}

} // namespace Input
} // namespace TestHelpers
