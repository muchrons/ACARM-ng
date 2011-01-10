/*
 * BackendFacadeStub.cpp
 *
 */
#include "TestHelpers/Input/BackendFacadeStub.hpp"

namespace TestHelpers
{
namespace Input
{

BackendFacadeStub::BackendFacadeStub(::Persistency::IO::ConnectionPtrNN  conn):
  ::Input::BackendFacade(conn, "testbackendfacade", ac_)
{
}

} // namespace Input
} // namespace TestHelpers
