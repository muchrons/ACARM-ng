/*
 * exports.mt.cpp
 *
 */
#include <iostream>
#include <stdexcept>
#include <boost/python.hpp>

#include "PythonAPI/Persistency/exports.hpp"

using namespace std;
using namespace boost::python;

namespace
{
static PythonAPI::Environment::StaticImporter g_importPersistency("persistency", PyInit_persistency);
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
    PythonAPI::Environment env;
    cout<<"running user's code..."<<endl;
    env.runFile(argv[1]);
    cout<<"done!"<<endl;
  }
  catch(const boost::python::error_already_set &)
  {
    cerr<<"got error message"<<endl;
    PyErr_Print();
    return 2;
  }

  return 0;
}
