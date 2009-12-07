/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_ALERT_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Alert.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Alert API
 */
class Alert: private boost::noncopyable
{
public:
  virtual ~Alert(void);

  virtual void save(const Persistency::Alert &alert) = 0;
  // TODO
}; // class Alert

} // namespace IO
} // namespace Persistency

#endif
