/*
 * Timestamp.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE
#define INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE

/* public header */

#include <boost/date_time/gregorian/gregorian.hpp>

namespace Persistency
{
/** \brief common date/time representation.
 */
typedef boost::gregorian::date Timestamp;

} // namespace Persistency

#endif
