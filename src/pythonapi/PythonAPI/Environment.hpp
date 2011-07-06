/*
 * Environment.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_ENVIRONMENT_HPP_FILE
#define INCLUDE_PYTHONAPI_ENVIRONMENT_HPP_FILE

/* public header */

#include <string>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "PythonAPI/Exception.hpp"
#include "PythonAPI/ExceptionFromScript.hpp"


namespace PythonAPI
{

/** \brief environment creation object.
 *
 *  instnace of this object has to be created in order to use python.
 *  multiple instances of this class can be used at once.
 *
 *  \warning this class is NOT THREAD SAFE. this is due to PythonC's and
 *           boost::python's limitations. use from one thread only!
 */
class Environment: private boost::noncopyable
{
public:
  /** \brief helper typedef for initialization function. */
  typedef PyObject* (*ModuleInitFunc)(void);

  /** \brief helper object for scheduing modulesinitialization.
   *
   *  when user's module is to be initializaed gobal instance of this object
   *  must be created. it will schedule import just before environment
   *  initialization (the first one).
   *
   *  this ensures no initialization races of global values are going to take
   *  places, while maintaining ability to use importing mechanism in a
   *  distributed way.
   */
  class StaticImporter
  {
  public:
    /** \brief schedules given module to importing.
     *  \param module module's name (must be non-NULL).
     *  \param init   initialization callback (must be non-NULL).
     *
     *  \note all instance of this class must be instanciated BEFORE first
     *        Environment's instance is created.
     *  \warning scheduling is NOT THREAD SAFE.
     */
    StaticImporter(const char *module, ModuleInitFunc init);
    /** \brief destroys given object.
     */
    ~StaticImporter(void);

  private:
    bool imported_;     // will be 0 (false) when declared static
  }; // struct StaticImporter


  /** \brief create environment to run embedded python's code.
   *
   *  creates runtime environment. will import user's modules and initilize
   *  python's interprenter, if not done so before.
   */
  Environment(void);

  /** \brief run given python code directly.
   *
   *  this call forward error rised in python scripts and throws them as
   *  ExceptionFromScript instances. more details can be obtained from it.
   *
   *  \param script script to be run (i.e. python code).
   */
  void run(const std::string &script);

  /** \brief gets variable from python.
   *
   *  this returns objects by value, which is not always expected. in particular
   *  it is not valid for classes, returned by pointers, since they may be
   *  salvaged by python's garbage collector. to get these the safe way use
   *  boost::shared_ptr<MyClass> as a destination type for MyClass pointers.
   *
   *  \note use boost::shared_ptr<MyClass> to take object with ownership.
   *
   *  \param name name of the variable to get.
   *  \return variable's value.
   */
  template<typename T>
  T var(const std::string &name)
  {
    return boost::python::extract<T>( (mainNamespace_[name]) );
  }

  /** \brief import regular, python's modules (i.e. not user defined ones).
   *  \param module name of the module to import (for example "sys").
   */
  void importModule(const std::string &module);

private:
  static void importModule(const std::string &module, ModuleInitFunc init);

  // implementation that wraps importModule(mod, init) calls.
  class ImportedModules;
  friend class ImportedModules;

  Logger::Node          log_;
  boost::python::object mainModule_;
  boost::python::object mainNamespace_;
}; // class ExceptionHandle

} // namespace PythonAPI

#endif
