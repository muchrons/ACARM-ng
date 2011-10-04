/*
 * NetIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NETIO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NETIO_HPP_FILE

#include <cstdlib>
#include <inttypes.h>
#include <boost/noncopyable.hpp>

#include "Persistency/IPTypes.hpp"
#include "Trigger/SnortSam/ExceptionCryptoFailed.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief communication channel API.
 */
class NetIO: private boost::noncopyable,
             public  Persistency::IPTypes<NetIO>
{
public:
  explicit NetIO(const std::string &host);

  /** \brief ensures polymorphic destruction.
   */
  virtual ~NetIO(void);

  /** \brief creates connection to a given host.
   *  \param data data to be encrypted.
   *  \param len  length of the data buffer to be encrypted.
   *  \return read-only reference to the internal buffer.
   */
  DataRef encrypt(const uint8_t *data, size_t len);
  /** \brief decrypts given data and stores result in the internal buffer.
   *  \param data data to be decrypted.
   *  \param len  length of the data buffer to be decrypted.
   *  \return read-only reference to the internal buffer.
   */
  DataRef decrypt(const uint8_t *data, size_t len);

private:
  DataRef getData(void) const;

  // must encrypt given data to the internal buffer
  virtual void encryptImpl(const uint8_t *data, size_t len) = 0;
  // must decrypt given data to the internal buffer
  virtual void decryptImpl(const uint8_t *data, size_t len) = 0;
  // must return reference (w/o ownership!) to the internal buffer
  virtual DataRef getDataImpl(void) const = 0;
}; // class Crypto

} // namespace SnortSam
} // namespace Trigger

#endif
