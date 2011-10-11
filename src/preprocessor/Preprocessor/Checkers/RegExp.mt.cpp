/*
 * RegExp.mt.cpp
 *
 */
#include <iostream>

#include "Preprocessor/Checkers/RegExp.hpp"

using namespace std;


int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <regex> <string>"<<endl;
    return 1;
  }

  const Preprocessor::Checkers::RegExp re(argv[1]);
  const bool ret=re.check(argv[2]);

  cout<<"re<"<<argv[1]<<">("<<argv[2]<<")=="<<(ret?"true":"false")<<endl;

  return 0;
}
