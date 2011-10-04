/*
 * Message.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_MESSAGE_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_MESSAGE_HPP_FILE

#include <cstdlib>
#include <inttypes.h>

#include "Persistency/IPTypes.hpp"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

/** \brief raw data package, as defined by SnortSam plugin.
 *
 *  this looks ugly and silently assumes no padding (which, according to standard,
 *  may be added between the fields). this is awful, but as long as
 *  architectures in use are x86-like, it should work. living it "as is".
 *
 *  this data structure must be POD in order to work.
 */
struct SamPacket
{
  uint16_t endianCheck_;
  uint8_t  srcIP_[4];
  uint8_t  dstIP_[4];
  uint8_t  duration_[4];
  uint8_t  snortSeqNo_[2];
  uint8_t  fwSeqNo_[2];
  uint8_t  srcPort_[2];
  uint8_t  dstPort_[2];
  uint8_t  protocol_[2];
  uint8_t  fwMode_;
  uint8_t  version_;
  uint8_t  status_;
  uint8_t  sigID_[4];
  uint8_t  fluff_;
}; // struct SamPacket


/** \brief helper class creating messages with a proper content.
 */
struct Message: public Persistency::IPTypes<Message>
{
  Message(void);

  void setIP(uint8_t (&out)[4], const IPv4 &ip);

  void setNum(uint8_t (&out)[2], uint16_t in);
  void setNum(uint8_t (&out)[4], uint32_t in);

  uint8_t *begin(void)
  {
    return static_cast<uint8_t*>( static_cast<void*>(&p_) );
  }
  const uint8_t *begin(void) const
  {
    return static_cast<const uint8_t*>( static_cast<const void*>(&p_) );
  }

  uint8_t *end(void)
  {
    return begin()+size();
  }
  const uint8_t *end(void) const
  {
    return begin()+size();
  }

  size_t size(void) const
  {
    return sizeof(p_);
  }

  SamPacket p_;
}; // struct Message

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
