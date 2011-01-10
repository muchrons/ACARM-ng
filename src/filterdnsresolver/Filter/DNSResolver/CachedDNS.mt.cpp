/*
 * CachedDNS.mt.cpp
 *
 */
#include <iostream>

#include "Filter/DNSResolver/CachedDNS.hpp"
#include "System/Timer.hpp"

using namespace std;
using namespace System;
using namespace Filter::DNSResolver;

int main(const int argc, const char **argv)
{
  if(argc<2)
  {
    cerr<<argv[0]<<" <ip1> <ip2> ... <ipN>"<<endl;
    return 2;
  }

  CachedDNS cache(120);

  for(int i=1; i<argc; ++i)
  {
    Timer                  t;
    const CachedDNS::IP    ip=CachedDNS::IP::from_string(argv[i]);
    const CachedDNS::Entry e =cache[ip];
    if(e.first)
      cerr<<argv[0]<<": IP '"<<ip<<"' maps to '"<<e.second.get()<<"'";
    else
      cerr<<argv[0]<<": no entry for '"<<ip<<"'";
    cerr<<" (found in "<<t.elapsed()<<"[s])"<<endl;
  }
}
