/*
 * TwoFish.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_TWOFISH_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_TWOFISH_HPP_FILE

#include <string>
#include <boost/scoped_ptr.hpp>

#include "Trigger/SnortSam/Crypto.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

/** \brief twofish cryptographic algorithm added.
 */
class TwoFish: public Crypto
{
public:
  explicit TwoFish(const std::string &key);
  virtual ~TwoFish(void);

private:
  // must encrypt given data to the internal buffer
  virtual void encryptImpl(const uint8_t *data, size_t len);
  // must decrypt given data to the internal buffer
  virtual void decryptImpl(const uint8_t *data, size_t len);
  // must return reference (w/o ownership!) to the internal buffer
  virtual DataRef getDataImpl(void) const;

  void resize(size_t len, bool decrypt);

  struct PImpl;
  boost::scoped_ptr<PImpl> impl_;
}; // class TwoFish

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
