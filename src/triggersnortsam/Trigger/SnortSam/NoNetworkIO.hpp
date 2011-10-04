/*
 * NoNetworkIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NONETWORKIO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NONETWORKIO_HPP_FILE

#include <vector>

#include "Trigger/SnortSam/NetIO.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief communication channel API.
 */
class NoNetworkIO: public NetIO
{
public:
  NoNetworkIO(void);

private:
  virtual bool isConnectedImpl(void);
  virtual void connectImpl(const IP &ip, uint16_t port);
  virtual void disconnectImpl(void);
  virtual void sendImpl(const uint8_t *data, size_t len, time_t deadline);
  virtual DataRef receiveImpl(size_t len, time_t deadline);

  std::vector<uint8_t> data_;
}; // class NoNetworkIO

} // namespace SnortSam
} // namespace Trigger

#endif
