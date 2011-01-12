/*
 * Downloader.mt.cpp
 *
 */
#include <iostream>

#include "Filter/IPBlackList/Downloader.hpp"

using namespace std;

int main(void)
{
  const Filter::IPBlackList::Downloader d(13);
  cout << d.download() << endl;
  return 0;
}
