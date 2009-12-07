/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_METAALERT_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::MetaAlert API
 */
class MetaAlert: private boost::noncopyable
{
public:
  virtual ~MetaAlert(void);

  virtual void save(const Persistency::MetaAlert &ma) = 0;

  virtual void updateSeverityDelta(const Persistency::MetaAlert &ma, double delta) = 0;

  virtual void updateCertanityDelta(const Persistency::MetaAlert &ma, double delta) = 0;

  // TODO
}; // class MetaAlert

} // namespace IO
} // namespace Persistency

#endif
