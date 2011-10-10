/*
 * BackendFacadeStub.cpp
 *
 */
#include "TestHelpers/Input/BackendFacadeStub.hpp"

using namespace Core::Types::Proc;

namespace TestHelpers
{
namespace Input
{

BackendFacadeStub::BackendFacadeStub(::Persistency::IO::ConnectionPtrNN  conn):
  ::Input::BackendFacade(conn, TypeName("testbackendfacade"), InstanceName("somename"), ac_)
{
}

} // namespace Input
} // namespace TestHelpers
