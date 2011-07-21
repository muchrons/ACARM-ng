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
//struct NonTerms: private System::NoInstance {};
//struct Terms: private System::NoInstance {};
struct OnCollectionAccess: private System::NoInstance {};
struct ErrorTests: private System::NoInstance {};

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
