/*
 * SeverityLevel.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SEVERITYLEVEL_HPP_FILE
#define INCLUDE_PERSISTENCY_SEVERITYLEVEL_HPP_FILE

/* public header */

#include "System/IterableEnum.hpp"

namespace Persistency
{
namespace detail
{
/** \brief helper type, implementation detail.
 */
struct SeverityLevelEnum
{
  /** \brief enum values.
   */
  typedef enum
  {
    DEBUG   =0,
    INFO    =1,
    LOW     =2,
    MEDIUM  =3,
    HIGH    =4
  }
  Type;
  /** \brief minimum value.
   */
  enum { Min=DEBUG };
  /** \brief maximum value.
   */
  enum { Max=HIGH };
}; // struct SeverityLevelEnum
} // namespace detail


/** \brief severity level interface (API).
 */
typedef System::IterableEnum<detail::SeverityLevelEnum> SeverityLevel;

} // namespace Persistency

#endif
