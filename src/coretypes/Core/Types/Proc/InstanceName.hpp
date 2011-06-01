/*
 * InstanceName.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_INSTANCENAME_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_INSTANCENAME_HPP_FILE

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
struct InstanceNameSpec
{
  /** \brief string  type to be used. */
  typedef std::string StringType;
}; // struct InstanceNameSpec
} // namespace detail

/** \brief type representing instance name of the processor. */
typedef System::UniqueTypeString<detail::InstanceNameSpec> InstanceName;

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
