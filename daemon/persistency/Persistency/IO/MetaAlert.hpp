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
  MetaAlert(Persistency::MetaAlertPtr  ma,
            const Transaction         &t);

  virtual ~MetaAlert(void);

  void save();

  void markAsUsed();

  void markAsUnused();

  void updateSeverityDelta(double delta);

  void updateCertanityDelta(double delta);

  void addChild(Persistency::MetaAlertPtr child);

  void associateWithAlert(Persistency::AlertPtr alert);

  // TODO
protected:
  const Persistency::MetaAlert &get(void) const;

private:
  virtual void saveImpl() = 0;
  virtual void markAsUsedImpl() = 0;
  virtual void markAsUnusedImpl() = 0;
  virtual void updateSeverityDeltaImpl(double delta) = 0;
  virtual void updateCertanityDeltaImpl(double delta) = 0;
  virtual void addChildImpl(Persistency::MetaAlertPtr child) = 0;
  virtual void associateWithAlertImpl(Persistency::AlertPtr alert) = 0;

  Persistency::MetaAlertPtr ma_;
}; // class MetaAlert


typedef std::auto_ptr<MetaAlert> MetaAlertAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
