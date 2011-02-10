/*
 * main.cpp
 *
 */
#include <iostream>
#include <cassert>

#include "Commons/Exception.hpp"
#include "Core/Main.hpp"
#include "AcarmNG/printBanner.hpp"

using namespace std;

int main(int /*argc*/, const char **argv)
{
  int ret=0;

  try
  {
    // fancy init ;)
    printBanner(argv[0]);
    Logger::Node log("main");

    try
    {
      LOGMSG_INFO(log, "starting application");
      Core::Main   m;                       // run application (all done in
                                            // background threads)
      LOGMSG_INFO(log, "system started");
      m.waitUntilDone();                    // wait until application is done
      LOGMSG_INFO(log, "system stopped normally");
    }
    catch(const Commons::Exception &ex)
    {
      ret=16;
      LOGMSG_FATAL_S(log) << argv[0] << ": exception (" << ex.getTypeName()
                          << ") caught: " << ex.what()
                          << "; exiting with code " << ret;
    }
    catch(const std::exception &ex)
    {
      ret=8;
      LOGMSG_FATAL_S(log) << argv[0] << ": std::exception caught: "
                          << ex.what() << "; exiting with code " << ret;
    }
    catch(...)
    {
      ret=4;
      LOGMSG_FATAL_S(log) << argv[0] << ": unknown exception caught; "
                                        "exiting with code " << ret;
    }
  }
  catch(const std::exception &ex)
  {
    ret=2;
    cerr<<argv[0]<<": std::exception caught: "<<ex.what()<<endl;
  }
  catch(...)
  {
    ret=1;
    cerr<<argv[0]<<": unknown exception caught"<<endl;
  }

  (ret==0?cout:cerr)<<argv[0]<<": exiting with return code "<<ret<<endl;
  if(ret!=0)
    cerr<<argv[0]<<": see logs for more details"<<endl;
  return ret;
}
