#include <iostream>
#include <string>
#include <boost/scoped_ptr.hpp>

#include "Logger/Appenders/File.hpp"

using namespace std;
using namespace boost;
using namespace Logger::Appenders;


int main(void)
{
  const string     file("test.log");
  scoped_ptr<Base> app( new File(file) );
  cout<<"you should see message below from "<<app->getTypeName()
      <<" appender in file "<<file<<endl;
  app->append("if you can read this it is ok :)");
  return 0;
}
