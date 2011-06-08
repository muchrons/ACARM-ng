/*
 * exports.mt.cpp
 *
 */
#include <vector>
#include <iostream>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/scoped_ptr.hpp>

#include "PythonAPI/Persistency/exports.hpp"

using namespace std;
using namespace boost::python;

int main(int argc, const char * const *argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <script.py>"<<endl;
    return 1;
  }

  try
  {
    if( PyImport_AppendInittab("persistency", PyInit_persistency)!=0 )
      throw std::runtime_error("unable to load 'persistency' module");

    cout<<"init..."<<endl;
    Py_Initialize();
    cout<<"import main..."<<endl;
    object mainModule=import("__main__");
    cout<<"import namespace..."<<endl;
    object mainNamespace=mainModule.attr("__dict__");
    cout<<"running user's code..."<<endl;
    exec_file(argv[1], mainNamespace);
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
