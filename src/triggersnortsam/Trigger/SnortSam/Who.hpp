/*
 * Who.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_WHO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_WHO_HPP_FILE

#include "System/Enum.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace detail
{
/** \brief implementation detail for enum.
 */
struct WhoEnum
{
  /** \brief enum value to be used. */
  typedef enum
  {
    NONE = 0,
    SRC  = 1,
    DST  = 2,
    BOTH = SRC|DST
  } Type;
}; // struct WhoEnum
} // namespace detail


/** \brief sepresentation of 'who' paramter. */
typedef System::Enum<detail::WhoEnum> Who;

/** \brief gets logical or value on two enum elements.
 *  \param l left side of the operator.
 *  \param r rigth side of the operator.
 *  \return value of the ored types.
 */
inline Who operator|(const Who l, const Who r)
{
  return static_cast<Who::Type>( l.toInt() | r.toInt() );
} // operator|()

} // namespace SnortSam
} // namespace Trigger

#endif
