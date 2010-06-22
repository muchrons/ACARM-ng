/*
 * SanityCheck.cpp
 *
 */
#include <unistd.h>
#include <sys/types.h>

#include "Core/SanityCheck.hpp"

namespace Core
{

SanityCheck::SanityCheck(void)
{
  ensureNotRoot();
}

void SanityCheck::ensureNotRoot(void)
{
  if( getuid()==0 )
    throw ExceptionRunningAsRoot(SYSTEM_SAVE_LOCATION);
}

} // namespace Core
