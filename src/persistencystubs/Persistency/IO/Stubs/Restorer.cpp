/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Stubs/Restorer.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

Restorer::Restorer(Transaction &t, int):
  IO::Restorer(t),
  restoreAllCalls_(0),
  restoreBetweenCalls_(0)
{
}

void Restorer::restoreAllInUseImpl(Transaction&, NodesVector&)
{
  ++restoreAllCalls_;
}

void Restorer::restoreBetweenImpl(Transaction&,
                                  NodesVector&,
                                  const Timestamp&,
                                  const Timestamp&)
{
  ++restoreBetweenCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
