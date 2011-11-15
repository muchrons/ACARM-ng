/*
 * main.cpp
 *
 */
#include <iostream>
#include <stdexcept>

#include "AcarmNG/MainImpl.hpp"

using namespace std;

int main(const int argc, char const * const * const argv)
{
  int ret=0;

  try
  {
    AcarmNG::MainImpl main(argc, argv);
    ret=main.run();
  }
  catch(const std::exception &ex)
  {
    ret=2;
    cerr<<argv[0]<<": std::exception caught: "<<ex.what()<<endl;
  }
  catch(...)
  {
    ret=1;
    cerr<<argv[0]<<": unknown exception caught"<<endl;
  }

  if(ret!=0)
  {
    cerr << argv[0] << ": exiting with return code " << ret << endl;
    cerr << argv[0] << ": see logs for more details" << endl;
  }

  return ret;
}
