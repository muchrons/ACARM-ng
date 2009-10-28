#include <boost/scoped_ptr.hpp>

#include "Logger/Appenders/Console.hpp"

using namespace boost;
using namespace Logger::Appenders;


int main(void)
{
  scoped_ptr<Base> app( new Console );
  app->append("if you can read this it is ok :)\n");
  return 0;
}
