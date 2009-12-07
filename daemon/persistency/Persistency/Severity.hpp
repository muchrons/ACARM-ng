/*
 * Severity.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SEVERITY_HPP_FILE
#define INCLUDE_PERSISTENCY_SEVERITY_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/SeverityLevel.hpp"

namespace Persistency
{

/** \brief severity representation.
 */
class Severity: private boost::noncopyable
{
public:
  /** \brief d-tor is virtual, since this class is intended to be used as a
   *         base for particular implementation.
   */
  virtual ~Severity(void);
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

protected:
  /** \brief crates class with a given severity.
   *  \param sl level to assign to this instance.
   */
  explicit Severity(SeverityLevel sl);

private:
  SeverityLevel sl_;
}; // class Severity


/** \brief smart pointer to severity level.
 */
typedef boost::shared_ptr<Severity> SeverityPtr;

} // namespace Persistency

#endif
