/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

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

  virtual void save() = 0;

  virtual void markAsUsed() = 0;

  virtual void markAsUnused() = 0;

  virtual void updateSeverityDelta(double delta) = 0;

  virtual void updateCertanityDelta(double delta) = 0;

  // TODO
protected:
  Persistency::MetaAlert &get(void);

private:
  Persistency::MetaAlertPtr ma_;
}; // class MetaAlert


typedef std::auto_ptr<MetaAlert> MetaAlertAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
