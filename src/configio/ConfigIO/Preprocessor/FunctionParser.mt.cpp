/*
 * FunctionParser.mt.cpp
 *
 */
#include <iostream>
#include <cassert>

#include "ConfigIO/Preprocessor/FunctionParser.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{

void print(const std::string &tab, const FormatterConfig::Wrapper &w)
{
  if( w.isValue() )
  {
    cout<<tab<<"value()"<<endl;
    return;
  }

  if( w.isArgument() )
  {
    cout<<tab<<",,"<<w.argument()<<"''"<<endl;
    return;
  }

  if( w.isFunction() )
  {
    cout<<tab<<w.name()<<endl;
    cout<<tab<<"  ("<<endl;
    for(size_t i=0; i<w.argCount(); ++i)
      print( tab+"    ", w.param(i) );
    cout<<tab<<"  )"<<endl;
    return;
  }

  assert(!"oops - wtf?");
}


void print(const FormatterConfig::Wrapper &w)
{
  print("", w);
}

} // unnamed namespace


int main(const int argc, char const * const * const argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <expr>"<<endl;
    return 1;
  }

  cout<<",,"<<argv[1]<<"'' -->"<<endl;
  const FunctionParser fp(argv[1]);
  print( fp.getConfig().get() );

  return 0;
}
