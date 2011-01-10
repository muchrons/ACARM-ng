/*
 * GlobalLibPreludeInit_singleGlobalInit.mt.cpp
 *
 */
#include <iostream>
#include "Input/Prelude/GlobalLibPreludeInit.hpp"

using namespace std;
using Input::Prelude::GlobalLibPreludeInit;

GlobalLibPreludeInit init;

int main(int, char **argv)
{
  cout<<argv[0]<<endl;
  return 0;
}
