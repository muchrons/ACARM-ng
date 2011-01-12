#include <iostream>
#include <boost/scoped_ptr.hpp>

#include "Logger/Appenders/Console.hpp"

using namespace boost;
using namespace Logger::Appenders;


int main(void)
{
  scoped_ptr<Logger::Appenders::Base> app( new Console );
  std::cout<<"you should see message below from "<<app->getTypeName()
           <<" appender"<<std::endl;
  app->append("if you can read this it is ok :)");
  return 0;
}
