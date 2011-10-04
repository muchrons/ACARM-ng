/*
 * twofish.t.cpp
 *
 */
#include <tut.h>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/SnortSam/Ver14/twofish.h"

namespace
{

void freeCharBuffer(char *ptr)
{
  if(ptr!=NULL)
    free(ptr);
} // freeCharBuffer()

typedef System::ScopedPtrCustom<TWOFISH, TwoFishDestroy> TWOFISHPtr;
typedef System::ScopedPtrCustom<char, freeCharBuffer>    MemoryPtr;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Ver14/twofish");
} // unnamed namespace


namespace tut
{

// try encrypting and decrypting some portion of data and see if it gives the same result
template<>
template<>
void testObj::test<1>(void)
{
  const char key[]="$3<r37";
  TWOFISHPtr fish( TwoFishInit(key) );
  ensure("initialization failed", fish.get()!=NULL );
  // encrypt
  const char  bufEnc[]="Cypher";
  char       *encBuf  =(char*)TwoFishAlloc(sizeof(bufEnc), false, false, fish.get());
  size_t      lenEnc  =TwoFishEncrypt(bufEnc, &encBuf, sizeof(bufEnc), false, fish.get());
  ensure("encryption failed", lenEnc!=0);
  // decrypt
  char        decBuf[sizeof(bufEnc)];
  char       *decBufPtr=decBuf;
  size_t      lenDec   =TwoFishDecrypt(encBuf, &decBufPtr, sizeof(decBuf)+TwoFish_BLOCK_SIZE, false, fish.get());
  ensure("decryption failed", lenDec!=0);
  ensure_equals("invalid size after decryption", lenDec, sizeof(bufEnc));
  for(size_t i=0; i<sizeof(bufEnc); ++i)
    ensure_equals("invalid char decoded", decBuf[i], bufEnc[i]);
}

} // namespace tut
