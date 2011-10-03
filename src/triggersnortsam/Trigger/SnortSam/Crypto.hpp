/*
 * Crypto.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_CRYPTO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_CRYPTO_HPP_FILE

#include <cstdlib>
#include <inttypes.h>
#include <boost/noncopyable.hpp>

#include "Trigger/SnortSam/ExceptionCryptoFailed.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief cryptographic API.
 *
 *  uses internal buffers for encryption/decryption. buffers are valid only until
 *  next call to any mmeber of the class.
 */
class Crypto: private boost::noncopyable
{
public:
  /** \brief gives read-only access to the internal buffer.
   */
  struct DataRef
  {
    DataRef(const uint8_t *data, const size_t len):
      data_(data),
      len_(len)
    {
    }

    const uint8_t *data_;
    size_t         len_;
  }; // struct DataRef

  /** \brief ensures polymorphic destruction.
   */
  virtual ~Crypto(void);

  /** \brief encrypts given data and stores result in the internal buffer.
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
