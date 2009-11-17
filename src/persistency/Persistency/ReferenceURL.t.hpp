/*
 * ReferenceURL.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_REFERENCEURL_T_HPP_FILE
#define INCLUDE_PERSISTENCY_REFERENCEURL_T_HPP_FILE

#include "Persistency/ReferenceURL.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct ReferenceURLTestImpl: public ReferenceURL
{
  ReferenceURLTestImpl(const char *name, const char *url):
    ReferenceURL(name, url)
  {
  }

  static ReferenceURLPtr makeNew(const char *url="http://gnu.org")
  {
    return ReferenceURLPtr( new ReferenceURLTestImpl("some name", url) );
  }
}; // struct ReferenceURLTestImpl

} // unnamed namespace

#endif
