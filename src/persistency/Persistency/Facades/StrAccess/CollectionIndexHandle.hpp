/*
 * CollectionIndexHandle.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_COLLECTIONINDEXHANDLE_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_COLLECTIONINDEXHANDLE_HPP_FILE

/* public header */

#include <cassert>
#include <boost/static_assert.hpp>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct CollectionIndexHandle: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( IsCollection<T>::value );
    assert(!p.isEnd());
    typedef typename TParams::template GetHandle<ErrorTests>::type ErrH;

    const size_t pos=Commons::Convert::to<size_t>(p.get());
    size_t       cur=0;
    for(typename T::const_iterator cit=e.begin(); cit!=e.end(); ++cit, ++cur)
    {
      if(cur==pos)  // found element with a given index? process it!
        return MainDispatcher::process(*cit, p);
    }

    ErrH::throwOnInvalidIndex(SYSTEM_SAVE_LOCATION, p);
    return false;   // this code is never reached
  }
}; // struct CollectionIndexHandle

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
