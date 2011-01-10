#include <iostream>
#include <boost/scoped_ptr.hpp>

#include "Logger/Appenders/Syslog.hpp"

using namespace boost;
using namespace Logger::Appenders;


int main(void)
{
  scoped_ptr<Logger::Appenders::Base> app(new Syslog);
  std::cout<<"you should see message from "<<app->getTypeName()
           <<" appender in system log"<<std::endl;
  app->append("ACARM-ng Syslog appender test - "
              "if you can read this it is ok :)");
  return 0;
}
