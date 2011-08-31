/*
 * exports.mt.cpp
 *
 */
#include <iostream>

#include "Commons/Filesystem/readTextFile.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace PythonAPI;
using namespace Trigger::Python;

int main(int argc, const char * const *argv)
{
  if(argc!=1+2+1)
  {
    cerr<<argv[0]<<" <th_pri> <th_count> <script.py>"<<endl;
    return 1;
  }

  try
  {
    cout<<"initializing..."<<endl;
    const Trigger::Simple::ThresholdConfig th(argv[1], argv[2]);
    const Config                           cfg(th, Config::Path(argv[3]));
    const Core::Types::Proc::InstanceName  name("mtest python script");
    cout<<"running user's code..."<<endl;
    Strategy s(name, cfg);

    cout<<"processing sample meta-alert..."<<endl;
    Trigger::Strategy::ChangedNodes cn;
    s.process(TestHelpers::Persistency::makeNewTree1(), cn);
    cout<<"done!"<<endl;
  }
  catch(const boost::python::error_already_set &)
  {
    cerr<<"got error message"<<endl;
    PyErr_Print();
    return 2;
  }
  catch(const std::exception &ex)
  {
    cerr<<"generic exception: "<<ex.what()<<endl;
    return 4;
  }

  return 0;
}
