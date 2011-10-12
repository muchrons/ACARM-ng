/*
 * NoCrypto.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NOCRYPTO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NOCRYPTO_HPP_FILE

#include <boost/scoped_array.hpp>

#include "Trigger/SnortSam/Crypto.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief API implementing no encryption at all
 */
class NoCrypto: public Crypto
{
public:
  NoCrypto(void);

private:
  virtual void encryptImpl(const uint8_t *data, size_t len);
  virtual void decryptImpl(const uint8_t *data, size_t len);
  virtual DataRef getDataImpl(void) const;

  void resize(const size_t len);

  boost::scoped_array<uint8_t> buf_;
  size_t                       size_;
  size_t                       used_;
}; // class NoCrypto

} // namespace SnortSam
} // namespace Trigger

#endif
