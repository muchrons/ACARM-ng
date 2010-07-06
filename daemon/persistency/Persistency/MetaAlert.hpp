/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_METAALERT_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Base/ObjectID.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/LimitedString.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/ReferenceURL.hpp"


namespace Persistency
{
namespace IO
{
// forward declaration for friend declaration.
class MetaAlert;
} // namespace IO

/** \brief meta-alert representation
 */
class MetaAlert: private boost::noncopyable,
                 public  boost::equality_comparable<MetaAlert>
{
public:
  /** \brief ID of a given meta-alert. */
  typedef Base::ObjectID<MetaAlert>   ID;
  /** \brief name for meta alert. */
  typedef Commons::LimitedString<256> Name;
  /** \brief severity difference type. */
  typedef double                      SeverityDelta;
  /** \brief certanity difference type. */
  typedef double                      CertaintyDelta;

  /** \brief creates meta alert based on exisitng alert.
   *  \param id    this object's ID.
   *  \param alert to corelate meta-alert with.
   */
  MetaAlert(const ID id, AlertPtrNN alert);

  /** \brief create new meta-alert.
   *  \param id             this object's ID.
   *  \param name           name for meta alert.
   *  \param severityDelta  initial severity difference.
   *  \param certanityDelta initial certanity difference.
   *  \param url            reference URL, if present.
   *  \param created        creation time.
   */
  MetaAlert(const ID         id,
            const Name      &name,
            SeverityDelta    severityDelta,
            CertaintyDelta   certanityDelta,
            ReferenceURLPtr  url,
            Timestamp        created);

  /** \brief gets meta-alert's ID.
   *  \return ID of meta alert.
   */
  ID getID(void) const;
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
  CertaintyDelta getCertaintyDelta(void) const;
  /** \brief gets reference url.
   *  \return reference url.
   */
  const ReferenceURL *getReferenceURL(void) const;
  /** \brief gets creation time of this meta alert.
   *  \return time of creation of this meta alert.
   */
  Timestamp getCreateTime(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const MetaAlert &other) const;

private:
  friend class IO::MetaAlert;
  void updateSeverityDelta(double delta);
  void updateCertaintyDelta(double delta);

  mutable Base::Threads::ReadWriteMutex mutex_;
  ID                                    id_;
  Name                                  name_;
  SeverityDelta                         severityDelta_;
  CertaintyDelta                        certanityDelta_;
  ReferenceURLPtr                       url_;
  Timestamp                             created_;
}; // class MetaAlert


/** \brief smart pointer to Alert type. */
typedef boost::shared_ptr<MetaAlert>         MetaAlertPtr;

/** \brief smart pointer to Alert type, checked not to be NULL. */
typedef Commons::SharedPtrNotNULL<MetaAlert> MetaAlertPtrNN;

} // namespace Persistency

#endif
