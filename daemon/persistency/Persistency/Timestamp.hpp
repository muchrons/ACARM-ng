/*
 * Timestamp.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE
#define INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE

/* public header */

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
// TODO: consider change to posix_time_types.hpp (smaller)

namespace Persistency
{
/** \brief common date/time representation.
 */
typedef boost::posix_time::ptime Timestamp;

} // namespace Persistency

#endif
