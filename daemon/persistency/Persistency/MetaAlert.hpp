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

#include "Base/Threads/Mutex.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/detail/LimitedString.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace Persistency
{
namespace IO
{
// forward declaration for friend declaration.
class MetaAlert;
} // namespace IO

/** \brief meta-alert representation
 */
class MetaAlert: private boost::noncopyable
{
public:
  /** \brief name for meta alert.
   */
  typedef detail::LimitedString<256> Name;
  /** \brief severity difference type.
   */
  typedef double                     SeverityDelta;
  /** \brief certanity difference type.
   */
  typedef double                     CertanityDelta;

  /** \brief creates meta alert based on exisitng alert.
   *  \param alert to corelate meta-alert with.
   */
  explicit MetaAlert(AlertPtr alert);

  /** \brief create new meta-alert.
   *  \param name           name for meta alert.
   *  \param severityDelta  initial severity difference.
   *  \param certanityDelta initial certanity difference.
   *  \param url            reference URL, if present.
   *  \param created        creation time.
   */
  MetaAlert(const Name      &name,
            SeverityDelta    severityDelta,
            CertanityDelta   certanityDelta,
            ReferenceURLPtr  url,
            Timestamp        created);

  /** \brief gets meta-alert's name.
   *  \return name of meta alert.
   */
  const Name &getName(void) const;
  /** \brief gets severity delta.
   *  \return severity delta.
   */
  SeverityDelta getSeverityDelta(void) const;
  /** \brief gets certanity delta.
   *  \return certanity delta.
   */
  CertanityDelta getCertanityDelta(void) const;
  /** \brief gets reference url.
   *  \return reference url.
   */
  const ReferenceURL *getReferenceURL(void) const;
  /** \brief gets creation time of this meta alert.
   *  \return time of creation of this meta alert.
   */
  Timestamp getCreateTime(void) const;

private:
  friend class IO::MetaAlert;
  void updateSeverityDelta(double delta);
  void updateCertanityDelta(double delta);

  mutable Base::Threads::Mutex mutex_;
  Name                         name_;
  SeverityDelta                severityDelta_;
  CertanityDelta               certanityDelta_;
  ReferenceURLPtr              url_;
  Timestamp                    created_;
}; // class MetaAlert


/** \brief smart pointer to Alert type.
 */
typedef boost::shared_ptr<MetaAlert> MetaAlertPtr;

} // namespace Persistency

#endif
