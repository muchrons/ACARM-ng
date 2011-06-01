/*
 * printBanner.cpp
 *
 */
#include <iostream>
#include <string>
#include <cassert>
#include <ctype.h>

#include "AcarmNG/printBanner.hpp"

using namespace std;

namespace AcarmNG
{

// helper function returning current year, based on compilation date
string getCurrentYear(void)
{
  const char *date=__DATE__;

  while(date[0]!=0 && date[1]!=0 && date[2]!=0 && date[3]!=0)
  {
    // do we have a year?
    if( isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) && isdigit(date[3]) )
    {
      const string year(date, date+4);
      assert( year.length()==4 );
      return year;
    }
    // if not, check next element
    ++date;
  } // while(date_can_still_fit)

  assert(!"we should never be here");
  return "????";    // unknown year...
} // getCurrentYear()

void printBanner(const char *app)
{
  assert(app!=NULL);
  // TODO: rework this code to take version from the VCS
  // TODO: rework this code to generate year automatically based on __DATE__ constant
  cout << app << ": ACARM-ng v0.0.0 (built on " << __DATE__ << " " << __TIME__ << ")" << endl;
  cout << app << ": copyright by WCSS (http://www.wcss.wroc.pl) 2009-" << getCurrentYear() << endl;
  cout << app << ": contact us: acarm@kdm.wcss.wroc.pl" << endl;
  cout << app << ":" << endl;
  cout << app << ": created by (in alphabetical order):" << endl;
  cout << app << ":   Bartłomiej 'Bartol' Balcerek (bartlomiej.balcerek@pwr.wroc.pl)" << endl;
  cout << app << ":   Bartosz 'BaSz' Szurgot (bartosz.szurgot@pwr.wroc.pl)" << endl;
  cout << app << ":   Mariusz 'Muchrons' Uchroński (mariusz.uchronski@pwr.wroc.pl)" << endl;
  cout << app << ":   Wojciech 'Wojek' Waga (wojciech.waga@pwr.wroc.pl)" << endl;
  cout << app << ":" << endl;
  cout << app << ": (see logs for details on application's current status/run)" << endl;
} // printBanner()

} // namespace AcarmNG
