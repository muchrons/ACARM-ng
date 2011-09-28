/*
 * How.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_HOW_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_HOW_HPP_FILE

#include "System/Enum.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace detail
{
/** \brief implementation detail for enum.
 */
struct HowEnum
{
  /** \brief enum value to be used. */
  typedef enum
  {
    NONE = 0,
    IN   = 1,
    OUT  = 2,
    BOTH = IN|OUT
  } Type;
}; // struct HowEnum
} // namespace detail


/** \brief sepresentation of 'how' paramter. */
typedef System::Enum<detail::HowEnum> How;

/** \brief gets logical or value on two enum elements.
 *  \param l left side of the operator.
 *  \param r rigth side of the operator.
 *  \return value of the ored types.
 */
inline How operator|(const How l, const How r)
{
  return static_cast<How::Type>( l.toInt() | r.toInt() );
} // operator|()

} // namespace SnortSam
} // namespace Trigger

#endif
