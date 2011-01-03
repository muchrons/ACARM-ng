/*
 * Loader.mt.cpp
 *
 */
#include <iostream>

#include "Plugins/Loader.hpp"

#include "Persistency/IO/BackendFactory.hpp"    
using namespace Persistency::IO;                

using namespace std;
using namespace Plugins;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <plugins_dir>"<<endl;
    return 1;
  }

  BackendFactory::unregisterBuilder("blablabla");       

  Loader loader(argv[1]);
  cout<<"loaded "<<loader.loadedCount()<<" plugins"<<endl;
  return 0;
}
