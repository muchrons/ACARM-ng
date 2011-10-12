/*
 * RegExp.mt.cpp
 *
 */
#include <iostream>
#include <stdexcept>
#include <cstring>

#include "Preprocessor/Checkers/RegExp.hpp"

using namespace std;


int main(int argc, char **argv)
{
  if(argc!=1+3)
  {
    cerr<<argv[0]<<" {--case-sensitive|--case-insensitive} <regex> <string>"<<endl;
    return 1;
  }
  const char *modeStr=argv[1];
  const char *regexp =argv[2];
  const char *str    =argv[3];

  bool caseSensitive;
  if( strcmp(modeStr, "--case-sensitive")==0 )
    caseSensitive=true;
  else
    if( strcmp(modeStr, "--case-insensitive")==0 )
      caseSensitive=false;
    else
      throw std::runtime_error("first option is invalid");

  const Preprocessor::Checkers::RegExp re(regexp, caseSensitive);
  const bool                           ret=re.check(str);

  cout<<"re"<<(caseSensitive?"":"-ci")<<"<"<<regexp<<">("<<str<<")=="<<(ret?"true":"false")<<endl;

  return 0;
}
