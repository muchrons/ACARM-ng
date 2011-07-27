/*
 * OnCollectionIndex.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONCOLLECTIONINDEX_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ONCOLLECTIONINDEX_HPP_FILE

/* public header */

#include <cassert>
#include <boost/static_assert.hpp>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"
#include "Persistency/Facades/StrAccess/collectionSize.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief processing facility handling collection indexes.
 */
struct OnCollectionIndex: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( IsCollection<T>::value );
    assert(!p.isEnd());
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;

    // query for collection's size
    if(p.get()=="size")
    {
      ErrH::throwIfNotLast(SYSTEM_SAVE_LOCATION, p);
      const size_t size=collectionSize(e.begin(), e.end());
      return p.callback().collectionSize(size);
    }

    // query for a given collection's element
    const size_t pos=Commons::Convert::to<size_t>(p.get());
    size_t       cur=0;
    for(typename T::const_iterator cit=e.begin(); cit!=e.end(); ++cit, ++cur)
    {
      if(cur==pos)  // found element with a given index? process it!
        return MainDispatcher::process(*cit, p);
    }

    // element of a given index not found
    ErrH::throwOnInvalidIndex(SYSTEM_SAVE_LOCATION, p);
    return false;   // this code is never reached
  }
}; // struct OnCollectionIndex

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
