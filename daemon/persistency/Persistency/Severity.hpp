/*
 * Severity.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SEVERITY_HPP_FILE
#define INCLUDE_PERSISTENCY_SEVERITY_HPP_FILE

/* public header */

#include <boost/operators.hpp>

#include "Persistency/SeverityLevel.hpp"

namespace Persistency
{

/** \brief severity representation.
 */
class Severity: public boost::equality_comparable<Severity>
{
public:
  /** \brief crates class with a given severity.
   *  \param sl level to assign to this instance.
   */
  explicit Severity(SeverityLevel sl);

  /** \brief gets severity level.
   *  \return severity level.
   */
  SeverityLevel getLevel(void) const;
  /** \brief gets string representation of a given severity.
   *  \return name of a given severity.
   *  \note string is held inside class, therefor pointer must NOT be
   *        deallocated nor used after class instance is destroyed.
   */
  const char *getName(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Severity &other) const;

private:
  SeverityLevel sl_;
}; // class Severity

} // namespace Persistency

#endif
