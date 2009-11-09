/*
 * Priority.hpp
 *
 */
#ifndef INCLUDE_LOGGER_PRIORITY_HPP_FILE
#define INCLUDE_LOGGER_PRIORITY_HPP_FILE

/* public header */

#include "System/IterableEnum.hpp"


namespace Logger
{
namespace detail
{
/** \brief helper class for System::IterableEnum<> template
 */
struct PriorityEnum
{
  /** \brief priorityies to be used.
   */
  typedef enum
  {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  } Type;
  /** \brief first element.
   */
  enum { Min=DEBUG };
  /** \brief last element.
   */
  enum { Max=FATAL };
}; // struct PriorityEnum
} // namespace detail


/** \brief Priority class to be used.
 */
typedef System::IterableEnum<detail::PriorityEnum> Priority;

} // namespace Logger

#endif
