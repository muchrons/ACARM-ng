/*
 * TypeName.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_TYPENAME_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_TYPENAME_HPP_FILE

/* public header */

#include <string>

#include "System/UniqueTypeString.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{

namespace detail
{
/** \brief internal helper.
 */
struct TypeNameSpec
{
  /** \brief string  type to be used. */
  typedef std::string StringType;
}; // struct TypeNameSpec
} // namespace detail

/** \brief type representing type name of the processor. */
typedef System::UniqueTypeString<detail::TypeNameSpec> TypeName;

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
