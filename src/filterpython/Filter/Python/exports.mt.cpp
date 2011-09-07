/*
 * exports.mt.cpp
 *
 */
#include <iostream>

#include "Commons/Filesystem/readTextFile.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Filter::Python;

int main(int argc, const char * const *argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <script.py>"<<endl;
    return 1;
  }

  try
  {
    cout<<"initializing..."<<endl;
    const Config                           cfg(120, argv[1]);
    const Core::Types::Proc::InstanceName  name("mtest python script");
    cout<<"running user's code..."<<endl;
    Strategy s(name, cfg);

    {
      cout<<"processing sample meta-alert (node)..."<<endl;
      Strategy::ChangedNodes cn;
      s.process(TestHelpers::Persistency::makeNewTree1(), cn);
      cout<<"changed nodes: "<<cn.size()<<endl;
    }

    {
      cout<<"processing sample meta-alert (leaf)..."<<endl;
      Strategy::ChangedNodes cn;
      s.process(TestHelpers::Persistency::makeNewLeaf(), cn);
      cout<<"changed nodes: "<<cn.size()<<endl;
    }

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
