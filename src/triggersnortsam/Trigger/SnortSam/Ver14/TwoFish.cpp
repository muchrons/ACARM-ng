/*
 * TwoFish.cpp
 *
 */
#include <cstdlib>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "System/EditableCString.hpp"
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
  explicit PImpl(const std::string &key)
  {
    // init algorithm with new password
    {
      System::EditableCString ecs(key.c_str());
      TWOFISHPtr tmp( TwoFishInit(ecs.get()) );
      if(tmp.get()==NULL)
        throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishInit() failed - unable to initialize crypto library");
      tfPtr_.swap(tmp);
    }
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

void TwoFish::encryptImpl(const uint8_t *data, size_t len)
{
  resize(len, false);
  // TODO
}

void TwoFish::decryptImpl(const uint8_t *data, size_t len)
{
  resize(len, true);
  // TODO
}

TwoFish::DataRef TwoFish::getDataImpl(void) const
{
  // TODO
  const uint8_t *ptr=reinterpret_cast<const uint8_t*>("");
  return DataRef(ptr, 0);
}

void TwoFish::resize(size_t len, bool decrypt)
{
  assert(impl_.get()!=NULL);
  void *mem=TwoFishAlloc(len, false, decrypt, impl_->tfPtr_.get());
  if(mem==NULL)
    throw ExceptionCryptoFailed(SYSTEM_SAVE_LOCATION, "TwoFishAlloc() failed - unable to allocate buffer");
  MemoryPtr tmp( reinterpret_cast<char*>(mem) );
  impl_->buf_.swap(tmp);
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
