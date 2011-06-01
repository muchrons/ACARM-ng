/*
 * Heartbeats.cpp
 *
 */
#include "Persistency/IO/Stubs/Heartbeats.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

Heartbeats::Heartbeats(const Owner &owner, Transaction &t, int /*h*/):
  Persistency::IO::Heartbeats(owner, t),
  reportCalls_(0u)
{
}

void Heartbeats::reportImpl(Transaction &, const Owner &, const Module &, Timestamp, unsigned int)
{
  ++reportCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
