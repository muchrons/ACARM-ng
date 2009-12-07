/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_METAALERT_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/detail/LimitedString.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace Persistency
{

/** \brief meta-alert representation
 */
class MetaAlert: private boost::noncopyable
{
public:
  typedef detail::LimitedString<256> Name;
  typedef boost::gregorian::date     Timestamp;
  typedef double                     SeverityDelta;
  typedef double                     CertanityDelta;

  /** \brief creates meta alert based on exisitng alert.
   *  \param alert to corelate meta-alert with.
   */
  //explicit MetaAlert(AlertPtr alert);

  MetaAlert(const Name      &name,
            ReferenceURLPtr  url,
            Timestamp        created);

  const Name &getName(void) const;

  SeverityDelta getSeverityDelta(void) const;

  CertanityDelta getCertanityDelta(void) const;

  const ReferenceURL *getReferenceURL(void) const;

  Timestamp getCreateTime(void) const;

  virtual Timestamp getLastUpdateTime(void) const = 0;

private:
  void updateSeverityDelta(double delta);
  void updateCertanityDelta(double delta);

  Name            name_;
  SeverityDelta   severityDelta_;
  CertanityDelta  certanityDelta_;
  ReferenceURLPtr url_;
  Timestamp       created_;
}; // class MetaAlert


/** \brief smart pointer to Alert type.
 */
typedef boost::shared_ptr<MetaAlert> MetaAlertPtr;

} // namespace Persistency

#endif
