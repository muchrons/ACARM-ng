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

void Heartbeats::report(const Module &m, const unsigned int timeout)
{
  if(timeout<1u)
    throw Exception(SYSTEM_SAVE_LOCATION, "timeout less than 1[s] does not make sense");
  t_.ensureIsActive();
  reportImpl(t_, owner_, m, Timestamp(), timeout);
}

} // namespace IO
} // namespace Persistency
