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
#include "Persistency/IO/DynamicConfig.hpp"


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
  /** \brief Object's ID. */
  typedef Base::ObjectID<MetaAlert>   ID;
  /** \brief name for meta alert. */
  typedef Commons::LimitedString<256> Name;
  /** \brief severity difference type. */
  typedef double                      SeverityDelta;
  /** \brief certanity difference type. */
  typedef double                      CertaintyDelta;

  /** \brief creates meta alert based on exisitng alert.
   *  \param alert to corelate meta-alert with.
   *  \param id    ID for this object.
   */
  MetaAlert(AlertPtrNN alert, ID id);

  /** \brief create new meta-alert.
   *  \param name           name for meta alert.
   *  \param severityDelta  initial severity difference.
   *  \param certanityDelta initial certanity difference.
   *  \param url            reference URL, if present.
   *  \param created        creation time.
   *  \param id             ID for this object.
   */
  MetaAlert(const Name      &name,
            SeverityDelta    severityDelta,
            CertaintyDelta   certanityDelta,
            ReferenceURLPtr  url,
            Timestamp        created,
            ID               id);

  /** \brief get ID of this object.
   *  \return object's ID.
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
  const ID                              id_;
  const Name                            name_;
  SeverityDelta                         severityDelta_;
  CertaintyDelta                        certanityDelta_;
  const ReferenceURLPtr                 url_;
  const Timestamp                       created_;
}; // class MetaAlert


/** \brief smart pointer to Alert type. */
typedef boost::shared_ptr<MetaAlert>         MetaAlertPtr;

/** \brief smart pointer to Alert type, checked not to be NULL. */
typedef Commons::SharedPtrNotNULL<MetaAlert> MetaAlertPtrNN;

} // namespace Persistency

#endif
