/*
 * ExceptionHostNameAlreadySaved.cpp
 *
 */
#include <cassert>

#include "Persistency/ExceptionHostNameAlreadySet.hpp"


namespace Persistency
{
ExceptionHostNameAlreadySet::ExceptionHostNameAlreadySet(const Location &where,
                                                         const char     *name):
  Exception(where, cc("host's name is already set to '", name, "'") )
{
  // when this exception is thrown name must be already set.
  assert( name!=NULL );
}

} // namespace Persistency
