/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Restorer.hpp"

namespace Persistency
{
namespace IO
{

Restorer::ExceptionInvalidTimeSpan::ExceptionInvalidTimeSpan(
                             const Location  &where,
                             const Timestamp &from,
                             const Timestamp &to):
  Exception(where, cc("invalid timestamp: from ", from, " to ", to) )
{
}

Restorer::~Restorer(void)
{
}

void Restorer::restoreAllInUse(NodesVector &out)
{
  restoreAllInUseImpl(t_, out);
}

void Restorer::restoreBetween(NodesVector     &out,
                              const Timestamp &from,
                              const Timestamp &to)
{
  if(to<from)
    throw ExceptionInvalidTimeSpan(SYSTEM_SAVE_LOCATION, from, to);
  restoreBetweenImpl(t_, out, from, to );
}

Restorer::Restorer(Transaction &t):
  t_(t)
{
}

} // namespace IO
} // namespace Persistency
