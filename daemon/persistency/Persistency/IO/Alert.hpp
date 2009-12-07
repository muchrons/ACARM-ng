/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_ALERT_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Alert API
 */
class Alert: private boost::noncopyable
{
public:
  Alert(Persistency::AlertPtr  alert,
        const Transaction     &t);

  virtual ~Alert(void);

  virtual void save(const Persistency::Alert &alert) = 0;
  // TODO

protected:
  Persistency::Alert &get(void);

private:
  Persistency::AlertPtr alert_;
}; // class Alert


typedef std::auto_ptr<Alert> AlertAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
