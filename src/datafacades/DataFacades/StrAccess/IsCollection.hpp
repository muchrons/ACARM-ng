/*
 * IsCollection.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_ISCOLLECTION_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_ISCOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "System/NoInstance.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Base/Threads/GrowingVector.hpp"
#include "Persistency/NodeChildrenVector.hpp"

namespace Facades
{
namespace StrAccess
{

/** \brief root template indicating if given object is known collection.
 *
 *  by default element is NOT a colleciton.
 *
 *  indireciton is not ensured - object passed bust be prepared if pointers are present.
 */
template<typename T>
struct IsCollection: private System::NoInstance
{
  static const bool value=false;    ///< meta-program result
}; // struct IsCollection

/** \brief makes std::vector<> a colection.
 */
template<typename T>
struct IsCollection< std::vector<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsCollection

/** \brief makes Base::Threads::GrowingVector<> a colection.
 */
template<typename T>
struct IsCollection< Base::Threads::GrowingVector<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsCollection

/** \brief makes Base::NonEmptyVector<> a colection.
 */
template<typename T>
struct IsCollection< Base::NonEmptyVector<T> >: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsCollection

/** \brief makes Persistency::NodeChildrenVector<> a colection.
 */
template<>
struct IsCollection<Persistency::NodeChildrenVector>: private System::NoInstance
{
  static const bool value=true;     ///< meta-program result
}; // struct IsCollection

} // namespace StrAccess
} // namespace Facades

#endif
