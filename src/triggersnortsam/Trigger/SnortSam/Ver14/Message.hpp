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
  uint16_t endianCheck_;    ///< assignment to this fields transfers info on endianness to the other side.
  uint8_t  srcIP_[4];       ///< source IP.
  uint8_t  dstIP_[4];       ///< destination IP.
  uint8_t  duration_[4];    ///< duration.
  uint8_t  snortSeqNo_[2];  ///< local sequence number.
  uint8_t  fwSeqNo_[2];     ///< remote sequence number.
  uint8_t  srcPort_[2];     ///< source port.
  uint8_t  dstPort_[2];     ///< destination port.
  uint8_t  protocol_[2];    ///< protocol type.
  uint8_t  fwMode_;         ///< how|how|log filed.
  uint8_t  version_;        ///< protocol version.
  uint8_t  status_;         ///< message type.
  uint8_t  sigID_[4];       ///< request (ban) ID.
  uint8_t  fluff_;          ///< this makes structure 32-bytes, on typical x86 hadrware.
}; // struct SamPacket


/** \brief helper class creating messages with a proper content.
 */
struct Message: public Persistency::IPTypes<Message>
{
  /** \brief create empty packet (onyl endianCHeck_ is set by deafult.
   */
  Message(void);
  /** \brief create object from already-created packet.
   */
  explicit Message(const SamPacket &p);

  /** \brief sets IP byte-field from given value.
   *  \param out destination to write data to.
   *  \param ip  source to read address from.
   */
  static void setIP(uint8_t (&out)[4], const IPv4 &ip);
  /** \brief gets IP from a given byte filed.
   *  \param in data to read from.
   *  \return converted IPv4.
   */
  static IPv4 getIP(const uint8_t (&in)[4]);

  /** \brief sets number's 2-byte-field from given value.
   *  \param out destination to write data to.
   *  \param in  source to read from.
   */
  static void setNum(uint8_t (&out)[2], uint16_t in);
  /** \brief gets number from a given 2-byte filed.
   *  \param in data to read from.
   *  \return converted number.
   */
  static uint16_t getNum(const uint8_t (&in)[2]);

  /** \brief sets number's 4-byte-field from given value.
   *  \param out destination to write data to.
   *  \param in  source to read from.
   */
  static void setNum(uint8_t (&out)[4], uint32_t in);
  /** \brief gets number from a given 4-byte filed.
   *  \param in data to read from.
   *  \return converted number.
   */
  static uint32_t getNum(const uint8_t (&in)[4]);

  /** \brief begin of data set.
   *  \return pointer to first byte in packet.
   */
  uint8_t *begin(void)
  {
    return static_cast<uint8_t*>( static_cast<void*>(&p_) );
  }
  /** \brief begin of data set.
   *  \return pointer to first byte in packet.
   */
  const uint8_t *begin(void) const
  {
    return static_cast<const uint8_t*>( static_cast<const void*>(&p_) );
  }

  /** \brief end of data iterator.
   *  \return iterator pointing to the end of data.
   */
  uint8_t *end(void)
  {
    return begin()+size();
  }
  /** \brief end of data iterator.
   *  \return iterator pointing to the end of data.
   */
  const uint8_t *end(void) const
  {
    return begin()+size();
  }

  /** \brief gets number of bytes in the packet.
   *  \return size of packet in bytes.
   */
  size_t size(void) const
  {
    return sizeof(p_);
  }

  SamPacket p_;     ///< package is avaiable directly, since it is easier to manipulate on it
}; // struct Message

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
