/*
 * Priority.hpp
 *
 */
#ifndef INCLUDE_LOGGER_PRIORITY_HPP_FILE
#define INCLUDE_LOGGER_PRIORITY_HPP_FILE

#include "System/IterableEnum.hpp"

namespace Logger
{
namespace detail
{
struct PriorityEnum
{
  typedef enum
  {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  } Type;
  enum { Min=DEBUG };
  enum { Max=FATAL };
}; // struct PriorityEnum
} // namespace detail

typedef System::IterableEnum<detail::PriorityEnum> Priority;

} // namespace Logger

#endif
