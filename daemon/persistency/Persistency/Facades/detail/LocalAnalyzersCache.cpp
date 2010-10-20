/*
 * LocalAnalyzersCache.cpp
 *
 */
#include <sstream>
#include <openssl/sha.h>

#include "Persistency/Exception.hpp"
#include "Persistency/Facades/detail/LocalAnalyzersCache.hpp"

using namespace std;

namespace Persistency
{
namespace Facades
{
namespace detail
{
namespace
{
// removes NULLs
const char *fixNulls(const char *in)
{
  if(in==NULL)
    return "<NULL>";
  return in;
} // cstrHash()

// converts half-byte to char
char hex2txt(const unsigned char in)
{
  assert(in<=0x0F);
  const char *lut="0123456789abcdef";
  assert( string(lut).length()==16 );
  return lut[in];
}

// converts binary form to text
void bin2txt(const unsigned char *in, size_t len, char *out)
{
  assert(in!=NULL);
  assert(out!=NULL);
  for(size_t i=0; i<len; ++i)
  {
    // higher half-byte
    *out=hex2txt( (*in)>>4 );
    ++out;
    // lower half-byte
    *out=hex2txt( (*in)&0x0F );
    ++out;
    // move input to next byte
    ++in;
  }
  *out=0;
} // bin2txt()

// pointer cast
const unsigned char *ptrConv(const char *c)
{
  // convert via 'void*'...
  return static_cast<const unsigned char*>( static_cast<const void*>(c) );
}

// computes (SHA1) hash of a given string
string computeHash(const string &in)
{
  // compute hash
  unsigned char binBuf[SHA_DIGEST_LENGTH];
  if( SHA1( ptrConv( in.c_str() ), in.length(), binBuf )!=binBuf )
    throw Exception(SYSTEM_SAVE_LOCATION, "unable to compute SHA1 hash; unknown error occured");
  // convert to txt
  char txtBuf[2*SHA_DIGEST_LENGTH +1];
  bin2txt(binBuf, SHA_DIGEST_LENGTH, txtBuf);
  assert( string(txtBuf).length()<sizeof(txtBuf) );
  // return value
  return txtBuf;
} // computeHash
} // unnamed namespace


LocalAnalyzersCache::Hash LocalAnalyzersCache::makeHash(const Analyzer::Name            &name,
                                                        const Analyzer::Version         &version,
                                                        const Analyzer::OperatingSystem &os,
                                                        const Analyzer::IP              *ip) const
{
  std::stringstream ss;
  ss << "Anlzr:HASH#";
  ss << fixNulls( name.get() );
  ss << "/";
  ss << fixNulls( version.get() );
  ss << "/";
  ss << fixNulls( os.get() );
  ss << "/";
  if(ip==NULL)
    ss << fixNulls(NULL);
  else
    ss << ip->to_string();

  return computeHash( ss.str() );
}

} // namespace detail
} // namespace Facades
} // namespace Persistency
