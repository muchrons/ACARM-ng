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
  /** \brief create stub.
   *  \param alert   alert to create IO for.
   *  \param t       active transaction.
   *  \param handler persistency handler.
   */
  Alert(Persistency::AlertPtr  alert,
        const Transaction     &t,
        int                    handler);

  /** number of times 'saveImpl()' has been called. */
  size_t saveCalls_;

private:
  virtual void saveImpl(void);
}; // class Alert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
