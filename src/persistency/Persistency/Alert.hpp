/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_ALERT_HPP_FILE

/* public header */

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "Persistency/Analyzer.hpp"
#include "Persistency/Severity.hpp"
#include "Persistency/Certanity.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace Persistency
{

/** \brief represents alert report got from input.
 */
class Alert: private boost::noncopyable
{
public:
  typedef detail::LimitedString<256> Name;
  typedef boost::gregorian::date     Timestamp;

  /** \brief starts virtual d-tors from the begining of the hierarchy.
   */
  virtual ~Alert(void);

  const Name &getName(void) const;

  const Analyzer &getAnalyzer(void) const;

  const Timestamp *getDetectionTime(void) const;

  const Timestamp &getCreationTime(void) const;

  const Severity &getSeverity(void) const;

  Certanity getCertanity(void) const;

  const std::string *getDescription(void) const;

protected:
  Alert(const Name        &name,
        AnalyzerPtr       &analyzer,
        const Timestamp   *detected,
        const Timestamp   &created,
        SeverityPtr        severity,
        Certanity          certanity,
        const std::string *description);

private:
  Name                           name_;
  AnalyzerPtr                    analyzer_;
  boost::scoped_ptr<Timestamp>   detected_;
  Timestamp                      created_;
  SeverityPtr                    severity_;
  Certanity                      certanity_;
  boost::scoped_ptr<std::string> description_;
}; // class Alert


/** \brief smart pointer to Alert type.
 */
typedef boost::shared_ptr<Alert> AlertPtr;

} // namespace Persistency

#endif
