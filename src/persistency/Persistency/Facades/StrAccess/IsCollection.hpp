/*
 * IsCollection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISCOLLECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_ISCOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "System/NoInstance.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Base/Threads/GrowingVector.hpp"
#include "Persistency/NodeChildrenVector.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

template<typename T>
struct IsCollection: private System::NoInstance
{
  static const bool value=false;
}; // struct IsCollection

template<typename T>
struct IsCollection< std::vector<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsCollection

template<typename T>
struct IsCollection< Base::Threads::GrowingVector<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsCollection

template<typename T>
struct IsCollection< Base::NonEmptyVector<T> >: private System::NoInstance
{
  static const bool value=true;
}; // struct IsCollection

template<>
struct IsCollection<NodeChildrenVector>: private System::NoInstance
{
  static const bool value=true;
}; // struct IsCollection

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
