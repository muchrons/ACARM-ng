/*
 * BackendFacade.cpp
 *
 */
#include <cassert>

#include "Input/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;

// TODO

namespace Input
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                            const std::string                 &name):
  Core::Types::BackendFacade(conn, name)
{
}

} // namespace Input
