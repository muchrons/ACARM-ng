/*
 * Conditional.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_CONDITIONAL_HPP_FILE
#define INCLUDE_BASE_THREADS_CONDITIONAL_HPP_FILE

/* public header */

#include <boost/thread/condition.hpp>

namespace Base
{
namespace Threads
{

/** \brief class to be used as a conditional type.
 */
typedef boost::condition_variable Conditional;

} // namespace Threads
} // namespace Base

#endif
