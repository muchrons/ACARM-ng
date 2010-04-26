/*
 * GlobalLibPreludeInit_multipleInit.mt.cpp
 *
 */
#include <iostream>
#include "Input/Prelude/GlobalLibPreludeInit.hpp"

using namespace std;
using Input::Prelude::GlobalLibPreludeInit;

int main(int, char **argv)
{
  GlobalLibPreludeInit init1;
  GlobalLibPreludeInit init2;
  cout<<argv[0]<<endl;
  return 0;
}
