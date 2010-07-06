/*
 * BackendFacadeStub.cpp
 *
 */
#include "TestHelpers/Input/BackendFacadeStub.hpp"

namespace TestHelpers
{
namespace Input
{
namespace detail
{
BackendFacadeStubData::BackendFacadeStubData(::Persistency::IO::ConnectionPtrNN  conn,
                                             ::Persistency::IO::Transaction     &t):
  cd_( new ::Input::CommonData(conn, t) )
{
}
} // namespace detail

BackendFacadeStub::BackendFacadeStub(::Persistency::IO::ConnectionPtrNN  conn,
                                     ::Persistency::IO::Transaction     &t):
  detail::BackendFacadeStubData(conn, t),
  ::Input::BackendFacade(conn, "the42", am_, cd_)
{
}

} // namespace Input
} // namespace TestHelpers
