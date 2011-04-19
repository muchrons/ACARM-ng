/*
 * Heartbeats.cpp
 *
 */
#include "Persistency/IO/Heartbeats.hpp"

namespace Persistency
{
namespace IO
{

Heartbeats::Heartbeats(const Owner &owner, Transaction &t):
  owner_(owner),
  t_(t)
{
  t_.ensureIsActive();
}

Heartbeats::~Heartbeats(void)
{
}

void Heartbeats::report(const Module &m, const time_t timeout)
{
  t_.ensureIsActive();
  reportImpl(t_, owner_, m, Timestamp(), timeout);
}

} // namespace IO
} // namespace Persistency
