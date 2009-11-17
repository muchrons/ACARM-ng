/*
 * HostRole.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_HOSTROLE_HPP_FILE
#define INCLUDE_PERSISTENCY_HOSTROLE_HPP_FILE

/* public header */

#include "System/Enum.hpp"

namespace Persistency
{
namespace detail
{
/** \brief helper type, implementation detail.
 */
struct HostRoleEnum
{
  /** \brief enum values.
   */
  typedef enum
  {
    SOURCE = 1,
    TARGET = 2
  } Type;
}; // struct HostRoleEnum
} // namespace detail


/** \brief host role interface (API).
 */
typedef System::Enum<detail::HostRoleEnum> HostRole;

} // namespace Persistency

#endif
