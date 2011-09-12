/*
 * CategoryName.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_CATEGORYNAME_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_CATEGORYNAME_HPP_FILE

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
struct CategoryNameSpec
{
  /** \brief string type to be used. */
  typedef std::string StringType;
}; // struct CategoryNameSpec
} // namespace detail

/** \brief type representing component's category name. */
typedef System::UniqueTypeString<detail::CategoryNameSpec> CategoryName;

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
