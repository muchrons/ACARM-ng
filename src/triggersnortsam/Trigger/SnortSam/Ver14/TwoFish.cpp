/*
 * TwoFish.cpp
 *
 */
#include <cstdlib>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/SnortSam/Ver14/TwoFish.hpp"
#include "Trigger/SnortSam/Ver14/twofish.h"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

// unnamed namespace
namespace
{
template<typename To, typename From>
To *convPtrTo(From *from)
{
  void *tmp=static_cast<void*>(from);
  return static_cast<To*>(tmp);
} // convPtrTo()

template<typename To, typename From>
const To *convPtrToConst(const From *from)
{
  const void *tmp=static_cast<const void*>(from);
  return static_cast<const To*>(tmp);
} // convPtrToConst()

void freeCharBuffer(char *ptr)
{
  if(ptr!=NULL)
    free(ptr);
} // freeCharBuffer()

typedef System::ScopedPtrCustom<TWOFISH, TwoFishDestroy> TWOFISHPtr;
typedef System::ScopedPtrCustom<char, freeCharBuffer>    MemoryPtr;
} // unnamed namespace


struct TwoFish::PImpl
{
  explicit PImpl(const std::string &key):
    tfPtr_( TwoFishInit(key.c_str()) ),
    len_(0)
  {
    if(tfPtr_.get()==NULL)
      throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishInit(): unable to initialize crypto library");
  }

  ~PImpl(void)
  {
    /* // TODO: required when TwoFishFree is used instead of TwoFishDestroy - check under valgrind and removed this code if not needed.
    // this is required, since library "generiously" deallocates
    // this memory for us, from time to time...
    if( static_cast<void*>(buf_.get())==static_cast<void*>(tfPtr_->output) )
      tfPtr_->output=NULL;
     */
  }

  TWOFISHPtr tfPtr_;
  MemoryPtr  buf_;
  size_t     len_;
};



TwoFish::TwoFish(const std::string &key):
  impl_(new PImpl(key))
{
  assert(impl_.get()!=NULL);
}

TwoFish::~TwoFish(void)
{
  // ensure safe destruction of PImpl
}

void TwoFish::encryptImpl(const uint8_t *data, const size_t len)
{
  resizeEnc(len);
  char         *tmp=impl_->buf_.get();
  const size_t  ret=TwoFishEncrypt( convPtrToConst<char>(data), &tmp, len ,false, impl_->tfPtr_.get() );
  assert(tmp==impl_->buf_.get());
  if(ret==0)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishEncrypt(): unable to encrypt message");
  impl_->len_=ret;
}

void TwoFish::decryptImpl(const uint8_t *data, const size_t len)
{
  resizeDec(len);
  char         *tmp=impl_->buf_.get();
  const size_t  ret=TwoFishDecrypt( convPtrToConst<char>(data), &tmp, len, false, impl_->tfPtr_.get() );
  assert(tmp==impl_->buf_.get());
  if(ret==0)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishDecrypt(): unable to decrypt message");
  impl_->len_=ret;
}

TwoFish::DataRef TwoFish::getDataImpl(void) const
{
  const uint8_t *ptr=convPtrToConst<uint8_t>( impl_->buf_.get() );
  return DataRef(ptr, impl_->len_);
}

void TwoFish::resizeEnc(const size_t len)
{
  assert(impl_.get()!=NULL);
  void *mem=TwoFishAlloc(len, false, false, impl_->tfPtr_.get());
  if(mem==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishAlloc(): unable to allocate buffer for encryption");
  MemoryPtr tmp( convPtrTo<char>(mem) );
  impl_->buf_.swap(tmp);
  impl_->len_=len;
}

void TwoFish::resizeDec(const size_t len)
{
  assert(impl_.get()!=NULL);
  void *mem=malloc(len);
  if(mem==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "malloc(): unable to allocate buffer for decryption");
  MemoryPtr tmp( convPtrTo<char>(mem) );
  impl_->buf_.swap(tmp);
  impl_->len_=len;
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
