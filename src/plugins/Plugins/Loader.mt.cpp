/*
 * Loader.mt.cpp
 *
 */
#include <iostream>

#include "Plugins/Loader.hpp"

using namespace std;
using namespace Plugins;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <plugins_dir>"<<endl;
    return 1;
  }

  Loader loader(argv[1]);
  cout<<"loaded "<<loader.loadedCount()<<" plugin(s)"<<endl;
  return 0;
}
