/*
 * Getter.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_GETTER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_GETTER_HPP_FILE

/* public header */

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/Path.hpp"
#include "Persistency/Facades/StrAccess/HandleMap.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief getter for a values specified by a given path.
 */
struct Getter: private System::NoInstance
{
}; // class struct

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
