/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::MetaAlert API
 */
class MetaAlert: private boost::noncopyable
{
public:
  /** \brief create meta-alert persistency proxy.
   *  \param ma meta alert to work on.
   *  \param t  active transaction.
   */
  MetaAlert(Persistency::MetaAlertPtrNN  ma,
            const Transaction           &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~MetaAlert(void);

  /** \brief saves meta alert to persistency storage.
   */
  void save();
  /** \brief save information that this obejct is in use now.
   */
  void markAsUsed();
  /** \brief mark object as not used anymore.
   */
  void markAsUnused();
  /** \brief update severity delta.
   *  \param delta how much update seveirty's value.
   */
  void updateSeverityDelta(double delta);
  /** \brief update certanity delta.
   *  \param delta how much update certanity's value.
   */
  void updateCertanityDelta(double delta);
  /** \brief save information that this meta-alert has another child.
   *  \param child meta-alert to be added as a child.
   */
  void addChild(Persistency::MetaAlertPtrNN child);
  /** \brief mark this meta-alert as associated with given alert.
   *  \param alert alter to be used as a leaf-node.
   */
  void associateWithAlert(Persistency::AlertPtrNN alert);

protected:
  /** \brief gets read-only access to meta-alert.
   */
  const Persistency::MetaAlert &get(void) const;

private:
  virtual void saveImpl() = 0;
  virtual void markAsUsedImpl() = 0;
  virtual void markAsUnusedImpl() = 0;
  virtual void updateSeverityDeltaImpl(double delta) = 0;
  virtual void updateCertanityDeltaImpl(double delta) = 0;
  virtual void addChildImpl(Persistency::MetaAlertPtrNN child) = 0;
  virtual void associateWithAlertImpl(Persistency::AlertPtrNN alert) = 0;

  Persistency::MetaAlertPtr ma_;
}; // class MetaAlert


typedef std::auto_ptr<MetaAlert> MetaAlertAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
