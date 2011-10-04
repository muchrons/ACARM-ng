/*
 * NoNetworkIO.cpp
 *
 */
#include "Trigger/SnortSam/NoNetworkIO.hpp"

namespace Trigger
{
namespace SnortSam
{

NoNetworkIO::NoNetworkIO(void):
  NetIO("127.0.0.1", 6666, 30)
{
}

bool NoNetworkIO::isConnectedImpl(void)
{
  return true;
}

void NoNetworkIO::connectImpl(const IP &/*ip*/, uint16_t /*port*/, time_t /*deadline*/)
{
}

void NoNetworkIO::disconnectImpl(void)
{
}

void NoNetworkIO::sendImpl(const uint8_t *data, size_t len, time_t /*deadline*/)
{
  assert(data!=NULL);
  data_.clear();
  for(size_t i=0; i<len; ++i)
    data_.push_back(data[i]);
}

DataRef NoNetworkIO::receiveImpl(size_t len, time_t /*deadline*/)
{
  if(len!=data_.size())
    throw std::runtime_error("invalid read request - length does not match saved data");
  return DataRef(&data_[0], len);
}

} // namespace SnortSam
} // namespace Trigger
