/*
 * printBanner.cpp
 *
 */
#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>
#include <ctype.h>

#include "ConfigConsts/version.hpp"
#include "AcarmNG/printBanner.hpp"

using namespace std;

namespace AcarmNG
{

namespace
{
unsigned int dig(const char d)
{
  assert('0'<=d);
  assert(d<='9');
  return d-'0';
}

// helper function returning current year, based on compilation date
unsigned int getCurrentYear(void)
{
  const char *date=__DATE__;

  while(date[0]!=0 && date[1]!=0 && date[2]!=0 && date[3]!=0)
  {
    // do we have a year?
    if( isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) && isdigit(date[3]) )
    {
      const unsigned int year=dig(date[0])*1000 + dig(date[1])*100 + dig(date[2])*10 + dig(date[3])*1;
      assert(year>=1970);
      return year;
    }
    // if not, check next element
    ++date;
  } // while(date_can_still_fit)

  assert(!"we should never be here");
  return 6666;      // unknown year...
} // getCurrentYear()


// print names of the authors in the random order
void printAuthors(std::ostream &os, const char *app)
{
  typedef std::vector<const char*> People;
  People ppl;
  ppl.push_back("Bartłomiej 'Bartol' Balcerek (bartlomiej.balcerek@pwr.wroc.pl)");
  ppl.push_back("Bartosz 'BaSz' Szurgot (bartosz.szurgot@pwr.wroc.pl)");
  ppl.push_back("Mariusz 'Muchrons' Uchroński (mariusz.uchronski@pwr.wroc.pl)");
  ppl.push_back("Wojciech 'Wojek' Waga (wojciech.waga@pwr.wroc.pl)");

  std::random_shuffle(ppl.begin(), ppl.end());

  for(People::const_iterator it=ppl.begin(); it!=ppl.end(); ++it)
  {
    assert(app!=NULL);
    assert(*it!=NULL);
    os << app << ":   " << *it << endl;
  }
} // printAuthors()
} // unnamed namespace


void printBanner(std::ostream &os, const char *app)
{
  assert(app!=NULL);
  const char *ver=ConfigConsts::versionString;
  os << app << ": ACARM-ng "<<ver<<" (built on " << __DATE__ << " " << __TIME__ << ")" << endl;
  os << app << ": copyright by WCSS (http://www.wcss.wroc.pl) 2009-" << getCurrentYear() << endl;
  os << app << ": http://www.acarm.wcss.wroc.pl" << endl;
  os << app << ": contact us: acarm@kdm.wcss.wroc.pl" << endl;
  os << app << ":" << endl;
  os << app << ": created by:" << endl;
  printAuthors(os, app);
  os << app << ":" << endl;
  os << app << ": (see logs for details on application's current status/run)" << endl;
} // printBanner()

} // namespace AcarmNG
