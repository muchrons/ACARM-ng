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
  MetaAlert(Persistency::MetaAlertPtr  ma,
            const Transaction         &t);

private:
  virtual void saveImpl();
  virtual void markAsUsedImpl();
  virtual void markAsUnusedImpl();
  virtual void updateSeverityDeltaImpl(double delta);
  virtual void updateCertanityDeltaImpl(double delta);
}; // class MetaAlert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
