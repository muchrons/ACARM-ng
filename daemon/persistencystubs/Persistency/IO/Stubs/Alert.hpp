/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_ALERT_HPP_FILE

#include "Persistency/IO/Alert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief operations on Persistency::Alert API
 */
class Alert: public IO::Alert
{
public:
  Alert(Persistency::AlertPtr  alert,
        const Transaction     &t,
        int                    handler);

private:
  virtual void saveImpl(const Persistency::Alert &alert);
}; // class Alert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
