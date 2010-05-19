/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE

/* public header */

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Exception.hpp"

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
            Transaction                 &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~MetaAlert(void);

  /** \brief saves meta alert to persistency storage.
   */
  void save();
  /** \brief save information that this obejct has been already triggered.
   *  \param name name of the triggering processor.
   */
  void markAsTriggered(const std::string &name);
  /** \brief save information that this obejct is in use now.
   */
  void markAsUsed();
  /** \brief mark object as not used anymore.
   *  \note calling this method cleans up markAsTriggered() marks too.
   */
  void markAsUnused();
  /** \brief update severity delta.
   *  \param delta how much update seveirty's value.
   */
  void updateSeverityDelta(double delta);
  /** \brief update certanity delta.
   *  \param delta how much update certanity's value.
   */
  void updateCertaintyDelta(double delta);
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
  Persistency::MetaAlertPtrNN get(void);

private:
  virtual void saveImpl(Transaction &t) = 0;
  virtual void markAsTriggeredImpl(Transaction &t, const std::string &name) = 0;
  virtual void markAsUsedImpl(Transaction &t) = 0;
  virtual void markAsUnusedImpl(Transaction &t) = 0;
  virtual void updateSeverityDeltaImpl(Transaction &t, double delta) = 0;
  virtual void updateCertaintyDeltaImpl(Transaction &t, double delta) = 0;
  virtual void addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child) = 0;
  virtual void associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert) = 0;

  Persistency::MetaAlertPtr  ma_;
  Transaction               &t_;
}; // class MetaAlert


/** \brief auto pointer to IO::MetaAlert class. */
typedef std::auto_ptr<MetaAlert> MetaAlertAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
