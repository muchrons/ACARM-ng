/*
 * Timestamp.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE
#define INCLUDE_PERSISTENCY_TIMESTAMP_HPP_FILE

/* public header */

#include <ostream>
#include <boost/operators.hpp>

namespace Persistency
{

/** \brief common date/time UTC representation as timestamp.
 */
class Timestamp: public boost::equality_comparable<Timestamp>,
                 public boost::less_than_comparable<Timestamp>,
                 public boost::equivalent<Timestamp>
{
public:
  /** \brief create current time represetnation.
   */
  Timestamp(void);

  /** \brief create timestamp from given time_t representation.
   *  \param ts timestamp to create from.
   */
  explicit Timestamp(time_t ts);

  /** \brief get timestamp value.
   *  \return timestamp value.
   */
  time_t get(void) const
  {
    return ts_;
  }

  /** \brief compare two timestamps.
   *  \param other timestamp to compare with.
   *  \return true if this timestamp is less than other timestamp, false otherwise.
   */
  bool operator<(const Timestamp &other) const
  {
    return get()<other.get();
  }

private:
  time_t ts_;
}; // class Timestamp


/** \brief stream output operator.
 *  \param os stream to output data to.
 *  \param ts timestamp to output.
 *  \return os stream.
 */
std::ostream &operator<<(std::ostream &os, const Timestamp ts);

} // namespace Persistency

#endif
