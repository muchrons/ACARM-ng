/*
 * printBanner.cpp
 *
 */
#include <iostream>
#include <cassert>

#include "AcarmNG/printBanner.hpp"

using namespace std;

namespace AcarmNG
{

void printBanner(const char *app)
{
  assert(app!=NULL);
  // TODO: rework this code to take version from the VCS
  // TODO: rework this code to generate year automatically based on __DATE__ constant
  cout << app << ": ACARM-ng v0.0.0 (built on " << __DATE__ << " " << __TIME__ << ")" << endl;
  cout << app << ": copyright by WCSS (http://www.wcss.wroc.pl) 2009-2010" << endl;
  cout << app << ": created by (in alphabetical order):" << endl;
  cout << app << ":   Bartłomiej 'Bartol' Balcerek (bartlomiej.balcerek@pwr.wroc.pl)" << endl;
  cout << app << ":   Bartosz 'BaSz' Szurgot (bartosz.szurgot@pwr.wroc.pl)" << endl;
  cout << app << ":   Mariusz 'Muchrons' Uchroński (mariusz.uchronski@pwr.wroc.pl)" << endl;
  cout << app << ":   Wojciech 'Wojek' Waga (wojciech.waga@pwr.wroc.pl)" << endl;
  cout << app << ":" << endl;
  cout << app << ": (see logs for details on application's current status/run)" << endl;
} // printBanner()

} // namespace AcarmNG
