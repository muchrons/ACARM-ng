/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_METAALERT_HPP_FILE

#include "Persistency/IO/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief operations on Persistency::MetaAlert API
 */
class MetaAlert: public IO::MetaAlert
{
public:
  /** \brief meta alert persistency IO.
   *  \param ma      meta alert to work on.
   *  \param t       active transaction.
   *  \param handler persistency handler.
   */
  MetaAlert(Persistency::MetaAlertPtrNN  ma,
            Transaction                 &t,
            int                          handler);

  size_t saveCalls_;                    ///< calss count to save().
  size_t markAsUsedCalls_;              ///< calss count to markAsUsed().
  size_t markAsUnusedCalls_;            ///< calss count to markAsUnused().
  size_t updateSeverityDeltaCalls_;     ///< calss count to updateSeverityDelta().
  size_t updateCertaintyDeltaCalls_;    ///< calss count to updateCertanityDelta().
  size_t addChildCalls_;                ///< calss count to addChild().
  size_t assoicateWithAlertCalls_;      ///< calss count to associateWithAlert().

private:
  virtual void saveImpl(Transaction &t);
  virtual void markAsUsedImpl(Transaction &t);
  virtual void markAsUnusedImpl(Transaction &t);
  virtual void updateSeverityDeltaImpl(Transaction &t, double delta);
  virtual void updateCertaintyDeltaImpl(Transaction &t, double delta);
  virtual void addChildImpl(Transaction &t, Persistency::MetaAlertPtrNN child);
  virtual void associateWithAlertImpl(Transaction &t, Persistency::AlertPtrNN alert);
}; // class MetaAlert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
