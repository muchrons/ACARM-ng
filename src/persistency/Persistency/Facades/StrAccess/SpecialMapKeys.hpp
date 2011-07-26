/*
 * SpecialMapKeys.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_SPECIALMAPKEYS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_SPECIALMAPKEYS_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

//struct CollectionTests: private System::NoInstance {};
//struct PointerTests: private System::NoInstance {};
struct OnNonTerm: private System::NoInstance {};
struct OnTerm: private System::NoInstance {};
struct OnCollectionIndex: private System::NoInstance {};
struct OnError: private System::NoInstance {};

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
