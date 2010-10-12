/*
 * Mode.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_MODE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_MODE_HPP_FILE

/* public header */

#include "System/Enum.hpp"

namespace Commons
{
namespace Filesystem
{
namespace detail
{
/** \brief helper structure.
 */
struct ModeEnum
{
  /** \brief enum representing modes. */
  typedef enum
  {
    READ =1,
    WRITE=2,
  } Type;
}; // struct ModeEnum
} // namespace detail


/** \brief class representing mode.
 */
typedef System::Enum<detail::ModeEnum> Mode;

} // namespace Filesystem
} // namespace Commons

#endif
