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

struct NonTermHandle: private System::NoInstance {};
struct TermHandle: private System::NoInstance {};
struct CollectionIndexHandle: private System::NoInstance {};
struct ErrorHandle: private System::NoInstance {};

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
