/*
 * Severity.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_SEVERITY_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_SEVERITY_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/Severity.hpp"

namespace Persistency
{
namespace Stubs
{
/** \brief severity representation.
 */
class Severity: public Persistency::Severity
{
public:
  /** \brief crates class with a given severity.
   *  \param sl level to assign to this instance.
   */
  explicit Severity(SeverityLevel sl):
    Persistency::Severity(sl)
  {
  }

  /** \brief creates sample severity.
   */
  Severity(void):
    Persistency::Severity(SeverityLevel::INFO)
  {
  }
}; // class Severity

} // namespace Stubs
} // namespace Persistency

#endif
