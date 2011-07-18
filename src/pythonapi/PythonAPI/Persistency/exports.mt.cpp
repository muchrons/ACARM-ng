/*
 * exports.mt.cpp
 *
 */
#include <iostream>

#include "Commons/Filesystem/readTextFile.hpp"
#include "PythonAPI/Persistency/exports.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace PythonAPI;

namespace
{
//static Environment::StaticImporter g_importPersistency("persistency", PyInit_persistency);
static Environment::StaticImporter g_importPersistency("persistency", initpersistency);
} // unnamed namespace

int main(int argc, const char * const *argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <script.py>"<<endl;
    return 1;
  }

  try
  {
    cout<<"initializing..."<<endl;
    Environment env;
    cout<<"reading program's code..."<<endl;
    const string code=Commons::Filesystem::readTextFile(argv[1]).get();
    cout<<"running user's code..."<<endl;
    env.run(code);
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
