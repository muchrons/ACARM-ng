/*
 * SpecialMapKeys.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_SPECIALMAPKEYS_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_SPECIALMAPKEYS_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"

namespace DataFacades
{
namespace StrAccess
{

/** \brief type indicating handle of non-terms in handle map. */
struct NonTermHandle: private System::NoInstance {};

/** \brief type indicating handle of terms in handle map. */
struct TermHandle: private System::NoInstance {};

/** \brief type indicating handle of collection indexes in handle map. */
struct CollectionIndexHandle: private System::NoInstance {};

/** \brief type indicating handle of error conditions in handle map. */
struct ErrorHandle: private System::NoInstance {};

/** \brief type indicating exception type thrown on invalid path. */
struct InvalidPathExceptionType: private System::NoInstance {};

} // namespace StrAccess
} // namespace DataFacades

#endif
