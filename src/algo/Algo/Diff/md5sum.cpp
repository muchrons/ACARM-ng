/*
 * md5sum.cpp
 *
 */
#include <cstring>
#include <cstdlib>

#include "Algo/Diff/md5sum.hpp"

namespace Algo
{
namespace Diff
{

Similarity compare(const Persistency::MD5Sum &e1, const Persistency::MD5Sum &e2)
{
  if(&e1==&e2)
    return 1;
  if( e1.get()==e2.get() )
    return 1;
  if( e1.get()==NULL || e2.get()==NULL )
    return 0;
  // full comparison
  return ( strcmp( e1.get(), e2.get() )==0 )?1:0;
} // compare()

} // namespace Diff
} // namespace Algo
