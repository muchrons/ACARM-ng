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
    NOTICE  =2,
    WARNING =3,
    PROBLEM =4,
    ERROR   =5,
    CRITICAL=6
  } Type;
  /** \brief minimum value.
   */
  enum { Min=DEBUG    };
  /** \brief maximum value.
   */
  enum { Max=CRITICAL };
}; // struct SeverityLevelEnum
} // namespace detail


/** \brief severity level interface (API).
 */
typedef System::IterableEnum<detail::SeverityLevelEnum> SeverityLevel;

} // namespace Persistency

#endif
