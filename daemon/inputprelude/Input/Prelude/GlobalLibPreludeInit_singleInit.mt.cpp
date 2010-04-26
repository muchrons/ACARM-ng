/*
 * GlobalLibPreludeInit_singleInit.mt.cpp
 *
 */
#include <iostream>
#include "Input/Prelude/GlobalLibPreludeInit.hpp"

using namespace std;
using Input::Prelude::GlobalLibPreludeInit;

int main(int, char **argv)
{
  GlobalLibPreludeInit init1;
  cout<<argv[0]<<endl;
  return 0;
}
