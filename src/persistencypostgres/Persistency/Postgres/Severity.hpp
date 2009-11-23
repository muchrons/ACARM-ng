/*
 * Severity.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_SEVERITY_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_SEVERITY_HPP_FILE

#include "Persistency/Severity.hpp"

namespace Persistency
{
namespace Postgres
{
/** \brief severity representation.
 */
class Severity: public Persistency::Severity
{
public:
  /** \brief crates class with a given severity.
   *  \param sl level to assign to this instance.
   */
  explicit Severity(SeverityLevel sl);
}; // class Severity

} // namespace Postgres
} // namespace Persistency

#endif
